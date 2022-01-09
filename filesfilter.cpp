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
{
    createFileList(included, excluded, mask, level);
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
        const level_t &level
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

                if (fileMaskMatched(entry, mask)) {
                    m_fileList.push_back(entry);
                }
            }
        } else {
            for (auto &entry : fs::directory_iterator(dir)) {

                if (fileMaskMatched(entry, mask)) {
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

bool SameFilesFinder::fileMaskMatched(
        const path_t &pathEntry,
        const mask_t &mask
        )
{
    if (!fs::is_regular_file(pathEntry)) {
        return false;
    }

    boost::smatch what;
    if (!boost::regex_match(pathEntry.filename().native(), what,
                            boost::regex(mask, boost::regex::icase))) {
        return false;
    }

    return true;
}

void SameFilesFinder::printPathContainer(
        const pathconteiner_t &pathContainer,
        std::ostream &os
        )
{
    for (const auto &path : pathContainer) {
        os << fs::canonical(path).native() << std::endl;
    }
}
