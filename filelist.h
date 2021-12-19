#pragma once

#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <iostream>

namespace fs = boost::filesystem;

namespace filelist {

using dirs_t = std::vector<std::string>;
using mask_t = std::vector<std::string>;
using filelist_t = std::list<std::string>;

//namespace fs = std::filesystem;

class FileListCreator
{
public:
    FileListCreator(
            const dirs_t &includeDirs //,
//            const dirs_t &excludeDirs,
//            const mask_t &mask = mask_t{}
            )
    {
        for (auto &incdir : includeDirs) {
            if (!fs::is_directory(incdir)) {
                std::cout << "Error!" << std::endl;
                return;
            }
            for (auto &path : fs::recursive_directory_iterator(incdir)) {
                if (fs::is_regular_file(path)) {
                    std::cout << path << std::endl;
                }
            }
        }
    }

//private:
//    filelist_t m_filelist;


};

}
