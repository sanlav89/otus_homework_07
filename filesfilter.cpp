#include "filesfilter.h"
#include <cassert>

using namespace filesfilter;

SameFilesFinder::SameFilesFinder(
        const filenames_t &included,
        const filenames_t &excluded,
        const mask_t &mask,
        const level_t &level,
        const size_t &minsize,
        const size_t &blocksize,
        const hashtype_t &hashtype
        )
    : m_blocksize(blocksize)
{
    createFileList(included, excluded, mask, level, minsize);
    findSameFiles(m_fileList, 0);
}

void SameFilesFinder::printFileList(std::ostream &os)
{
    printPathContainer(m_fileList, os);
}

void SameFilesFinder::printSameFiles(std::ostream &os)
{
    for (const auto &group : m_sameFilesGroups) {
        printPathContainer(group, os);
    }
}

void SameFilesFinder::createFileList(
        const filenames_t &included,
        const filenames_t &excluded,
        const mask_t &mask,
        const level_t &level,
        const size_t &minsize
        )
{
    pathconteiner_t includedPaths;
    pathconteiner_t excludedPaths;
    m_fileList.clear();

    for (const auto &filename : included) {
        includedPaths.push_back(fs::canonical(path_t(filename)));
    }

    for (const auto &filename : excluded) {
        excludedPaths.push_back(fs::canonical(path_t(filename)));
    }

    for (auto &dir : includedPaths) {

        // watch if dir is in excluded list
        if (contains(excludedPaths, dir)) {
            continue;
        }

        if (level == All) {
            for (auto &entry : fs::recursive_directory_iterator(dir)) {

                // watch if entry is in excluded list
                if (contains(excludedPaths, dir)) {
                    continue;
                }

                if (filtered(entry, mask, minsize)) {
                    m_fileList.push_back(entry);
                }
            }
        } else {
            for (auto &entry : fs::directory_iterator(dir)) {

                if (filtered(entry, mask, minsize)) {
                    m_fileList.push_back(entry);
                }

            }
        }
    }
}

bool SameFilesFinder::contains(
        const pathconteiner_t &pathContainer,
        const path_t &pathEntry
        )
{
    return std::find(pathContainer.begin(), pathContainer.end(), pathEntry)
            != pathContainer.end();
}

bool SameFilesFinder::filtered(
        const path_t &pathEntry,
        const mask_t &mask,
        const size_t &minsize
        )
{
    // check if path is a file
    if (!fs::is_regular_file(pathEntry)) {
        return false;
    }

    // check if filesize >= minsize
    if (fs::file_size(pathEntry) < minsize) {
        return false;
    }

    // check if mask matched
    boost::smatch what;
    if (!boost::regex_match(pathEntry.filename().string(), what,
                            boost::regex(mask, boost::regex::icase))) {
        return false;
    }

    return true;
}

void SameFilesFinder::findSameFiles(
        const pathconteiner_t &filelist,
        size_t blockNumber
        )
{
    std::map<hash::Md5, pathconteiner_t> map;
    pathconteiner_t sameFiles;
    for (const auto &filename : filelist) {
        if (canCreateHash(filename, blockNumber)) {
            map[hashOfFileBlock(filename, blockNumber)].push_back(filename);
        } else {
            sameFiles.push_back(filename);
        }
    }

    if (sameFiles.size() > 1) {
        m_sameFilesGroups.push_back(sameFiles);
    }

    for (auto key : map) {
        if (key.second.size() > 1) {
            findSameFiles(key.second, blockNumber + 1);
        }
    }
}

bool SameFilesFinder::canCreateHash(const path_t &filename, size_t blockNumber)
{
    return blockNumber * m_blocksize < fs::file_size(filename);
}

hash::Md5 SameFilesFinder::hashOfFileBlock(
        const path_t &filename,
        size_t blockNumber
        )
{
    assert(blockNumber * m_blocksize <= fs::file_size(filename));

    // fstream settings
    std::ifstream file(filename.string(), std::ios::binary | std::ios::ate);
    file.seekg(blockNumber * m_blocksize, std::ios::beg);
    std::vector<char> buffer(m_blocksize);
    auto size = m_blocksize;

    // if last part is not multiple, set to zero the tail of block
    if ((blockNumber + 1) * m_blocksize > fs::file_size(filename)) {
        size = fs::file_size(filename) - blockNumber * m_blocksize;
        std::memset(buffer.data(), 0, m_blocksize);
    }

    // Read data to buffer
    if (!file.read(buffer.data(), size)) {
        throw std::runtime_error("Error reading file " + filename.string());
    }

    return hash::Md5(buffer.data(), m_blocksize);
}

void SameFilesFinder::printPathContainer(
        const pathconteiner_t &pathContainer,
        std::ostream &os
        )
{
    for (const auto &path : pathContainer) {
        os << fs::canonical(path).string()
           << " size: " << fs::file_size(path)
           << std::endl;
    }
    os << std::endl;
}
