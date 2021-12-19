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

namespace filelist {

using dirs_t = std::vector<std::string>;
using mask_t = std::vector<std::string>;
using paths_t = std::vector<fs::path>;
using filelist_t = std::list<std::string>;

class FileListCreator
{
public:
    FileListCreator(
            const dirs_t &includeDirs,
            const dirs_t &excludeDirs = {}/*,
            const mask_t &mask = {}*/
            )
    {
        filterInputData(includeDirs, excludeDirs);
/*
        static const boost::regex e("<(From )?([A-Za-z0-9_]+)>(.*?)");
        std::string chat_input("<Darker> Hello");
        boost::match_results<std::string::const_iterator> results;
        if (boost::regex_match(chat_input, results, e))
        {
             std::string playername = results[2];  //Get contents of the second (...)
        }
*/
        for (auto &incdir : includeDirs) {
            for (auto &entry : fs::recursive_directory_iterator(incdir)) {

                if (!fs::is_regular_file(entry)) {
                    continue;
                }

                boost::smatch what;

//                if (!boost::regex_match(entry.path().filename().native(), what, boost::regex("[5]"))) {
                if (!boost::regex_match(entry.path().filename().native(), what, boost::regex(".*\Pack.*\cmake"))) {
                    continue;
                }

//                if (fs::is_regular_file(entry) && isFilteredByMask(entry, mask)) {
                    std::cout << fs::canonical(entry) << std::endl;
//                }
            }
        }
    }

private:
    paths_t m_incPaths;
    paths_t m_excPaths;

    void filterInputData(const dirs_t &includeDirs, const dirs_t &excludeDirs)
    {
        bool same = false;
        for (const auto &incName : includeDirs) {
            fs::path incPath = absoluteDirPath(incName);
            same = false;
            for (const auto &excName : excludeDirs) {
                fs::path excPath = absoluteDirPath(excName);
                if (excPath.compare(incPath) == 0) {
                    same = true;
                    break;
                } else if (isSubPath(incPath, excPath)) {
                    m_excPaths.push_back(excPath);
                }
            }
            if (!same) {
                m_incPaths.push_back(incPath);
            }
        }
    }

    static bool isSubPath(const fs::path &mainPath, const fs::path &subPath)
    {
        return isSubString(mainPath.native(), subPath.native());
    }

    static bool isSubString(const std::string &mainStr, const std::string &subStr)
    {
        return mainStr.find(subStr) != std::string::npos;
    }

    static fs::path absoluteDirPath(const fs::path &path)
    {
        fs::path result = fs::canonical(path);
        if (!fs::is_directory(path)) {
            throw path.native() + " is not a directory!";
        }
        return result;
    }

    static bool isFilteredByMask(const fs::path &file, const mask_t &mask)
    {
        if (mask.empty()) {
            return true;
        }
        for (const auto &part : mask) {
            if (!isSubString(file.native(), part)) {
                return false;
            }
        }
        return true;
    }

//    void toAbsolutePaths(const dirs_t &paths)
//    {
//        dirs_t result;
//        for (const auto &)
//        return result;
//    }

//    static bool is_subdirectory()

//    filelist_t m_filelist;


};

}
