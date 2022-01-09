#pragma once

#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>

namespace fs = boost::filesystem;

namespace filesfilter {

using filenames_t = std::vector<std::string>;
using path_t = fs::path;
using pathconteiner_t = std::list<path_t>;
using mask_t = std::string;

enum ScanLevel {
    Current,
    All
};

enum HashType {
    Md5,
    Crc32
};

using level_t = ScanLevel;
using hashtype_t = HashType;

class SameFilesFinder
{
public:
    SameFilesFinder(
            const filenames_t &included,
            const filenames_t &excluded,
            const mask_t &mask,
            const level_t &level,
            const size_t &minsize,
            const size_t &blocksize,
            const hashtype_t &hashtype
            );

    void printFileList(std::ostream &os = std::cout);
    void printSameFiles(std::ostream &os = std::cout);

private:

    pathconteiner_t m_fileList;
    pathconteiner_t m_sameFiles;

    void createFileList(
            const filenames_t &included,
            const filenames_t &excluded,
            const mask_t &mask,
            const level_t &level
            );

    // void findSameFiles();

    bool contains(const pathconteiner_t &pathContainer, const path_t &pathEntry);
    bool fileMaskMatched(const path_t &pathEntry, const mask_t &mask);

    static void printPathContainer(const pathconteiner_t &pathContainer, std::ostream &os = std::cout);

};

}
