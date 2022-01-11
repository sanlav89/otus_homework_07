#include "filesfilter.h"

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
}

void SameFilesFinder::printFileList(std::ostream &os)
{
    printPathContainer(m_fileList, os);
}

void SameFilesFinder::printSameFiles(std::ostream &os)
{
    printPathContainer(m_sameFiles, os);
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
    std::map<hash::Md5, path_t> map;
    for (const auto &filename : filelist) {
        map[hashOfFileBlock(filename, blockNumber)] = filename;
    }

    pathconteiner_t nextFileList;
    auto iter = map.begin();
    auto prev = (*iter).first;
    iter++;
    nextFileList.push_back((*iter).second);
    while (iter != map.end()) {
        if ((*iter).first != prev) {
            if (nextFileList.size() > 1) {
                findSameFiles(nextFileList, blockNumber + 1);
            }
            nextFileList.clear();
        }
        prev = (*iter).first;
        nextFileList.push_back((*iter).second);
        iter++;
    }
}

hash::Md5 SameFilesFinder::hashOfFileBlock(
        const path_t &filename,
        size_t blockNumber
        )
{

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
}
