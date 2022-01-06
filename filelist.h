#pragma once

#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/regex.hpp>
#include <iostream>

namespace fs = boost::filesystem;

namespace scan {

using filenames_t = std::vector<std::string>;
using mask_t = std::string;
using paths_t = std::list<fs::path>;
using filelist_t = std::list<std::string>;

enum ScanLevel {
    Current,
    All
};

using level_t = ScanLevel;

paths_t fileList(
        const filenames_t &included = {"./"},
        const filenames_t &excluded = {},
        const ScanLevel &level = Current,
        const mask_t &mask = {}
        )
{
    paths_t result;
    paths_t includedPaths;
    paths_t excludedPaths;

    for (const auto &filename : included) {
        includedPaths.push_back(fs::canonical(fs::path(filename)));
    }

    for (const auto &filename : excluded) {
        excludedPaths.push_back(fs::canonical(fs::path(filename)));
    }

    for (auto &dir : includedPaths) {

        // watch if dir is in excluded list
        if (std::find(excludedPaths.begin(), excludedPaths.end(), dir) != excludedPaths.end()) {
            continue;
        }

        if (level == All) {
            for (auto &entry : fs::recursive_directory_iterator(dir)) {

                // watch if entry is in excluded list
                if (std::find(excludedPaths.begin(), excludedPaths.end(), dir) != excludedPaths.end()) {
                    continue;
                }

                if (fs::is_regular_file(entry)) {
                    result.push_back(entry);
                }
            }
        } else {
            for (auto &entry : fs::directory_iterator(dir)) {

                if (fs::is_regular_file(entry)) {
                    result.push_back(entry);
                }

            }
        }

    }

    return result;
}

void printFileList(const paths_t &fileList, std::ostream &os = std::cout)
{
    for (const auto &path : fileList) {
        os << fs::canonical(path).native() << std::endl;
    }
}


/*
bool isSubString(const std::string &mainStr, const std::string &subStr)
{
    return mainStr.find(subStr) != std::string::npos;
}

bool isSubPath(const fs::path &mainPath, const fs::path &subPath)
{
    return isSubString(fs::canonical(mainPath).native(),
                       fs::canonical(subPath).native());
}
*/


//class FileListCreator
//{
//public:
//    FileListCreator(
//            const dirs_t &includeDirs,
//            const dirs_t &excludeDirs = {}/*,
//            const mask_t &mask = {}*/
//            )
//    {
//        filterInputData(includeDirs, excludeDirs);
///*
//        static const boost::regex e("<(From )?([A-Za-z0-9_]+)>(.*?)");
//        std::string chat_input("<Darker> Hello");
//        boost::match_results<std::string::const_iterator> results;
//        if (boost::regex_match(chat_input, results, e))
//        {
//             std::string playername = results[2];  //Get contents of the second (...)
//        }
//*/
//        for (auto &incdir : includeDirs) {
//            for (auto &entry : fs::recursive_directory_iterator(incdir)) {

//                if (!fs::is_regular_file(entry)) {
//                    continue;
//                }

//                boost::smatch what;

////                if (!boost::regex_match(entry.path().filename().native(), what, boost::regex("[5]"))) {
//                if (!boost::regex_match(entry.path().filename().native(), what,
//                                        boost::regex(".*\pack.*\cmake", boost::regex::icase))) {
//                    continue;
//                }

////                if (fs::is_regular_file(entry) && isFilteredByMask(entry, mask)) {
//                    std::cout << fs::canonical(entry) << std::endl;
////                }
//            }
//        }
//    }

//private:
//    paths_t m_incPaths;
//    paths_t m_excPaths;

//    void filterInputData(const dirs_t &includeDirs, const dirs_t &excludeDirs)
//    {
//        bool same = false;
//        for (const auto &incName : includeDirs) {
//            fs::path incPath = absoluteDirPath(incName);
//            same = false;
//            for (const auto &excName : excludeDirs) {
//                fs::path excPath = absoluteDirPath(excName);
//                if (excPath.compare(incPath) == 0) {
//                    same = true;
//                    break;
//                } else if (isSubPath(incPath, excPath)) {
//                    m_excPaths.push_back(excPath);
//                }
//            }
//            if (!same) {
//                m_incPaths.push_back(incPath);
//            }
//        }
//    }

//    static bool isSubPath(const fs::path &mainPath, const fs::path &subPath)
//    {
//        return isSubString(mainPath.native(), subPath.native());
//    }

//    static bool isSubString(const std::string &mainStr, const std::string &subStr)
//    {
//        return mainStr.find(subStr) != std::string::npos;
//    }

//    static fs::path absoluteDirPath(const fs::path &path)
//    {
//        fs::path result = fs::canonical(path);
//        if (!fs::is_directory(path)) {
//            throw path.native() + " is not a directory!";
//        }
//        return result;
//    }

//    static bool isFilteredByMask(const fs::path &file, const mask_t &mask)
//    {
//        if (mask.empty()) {
//            return true;
//        }
//        for (const auto &part : mask) {
//            if (!isSubString(file.native(), part)) {
//                return false;
//            }
//        }
//        return true;
//    }

////    void toAbsolutePaths(const dirs_t &paths)
////    {
////        dirs_t result;
////        for (const auto &)
////        return result;
////    }

////    static bool is_subdirectory()

////    filelist_t m_filelist;


//};

}
