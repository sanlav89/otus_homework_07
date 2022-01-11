#pragma once

#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <string>
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include "hash.h"

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
    std::list<pathconteiner_t> m_sameFilesGroups;
    size_t m_blocksize;

    void createFileList(
            const filenames_t &included,
            const filenames_t &excluded,
            const mask_t &mask,
            const level_t &level,
            const size_t &minsize
            );

    bool contains(
            const pathconteiner_t &pathContainer,
            const path_t &pathEntry
            );

    bool filtered(
            const path_t &pathEntry,
            const mask_t &mask,
            const size_t &minsize
            );

    void findSameFiles(const pathconteiner_t &filelist, size_t blockNumber);

    bool canCreateHash(const path_t &filename, size_t blockNumber);

    hash::Md5 hashOfFileBlock(const path_t &filename, size_t blockNumber);

    static void printPathContainer(
            const pathconteiner_t &pathContainer,
            std::ostream &os = std::cout
            );

};

}
