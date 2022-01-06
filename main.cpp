#include <boost/program_options.hpp>
#include <iostream>
#include <list>
#include <set>
#include <algorithm>

#include "filelist.h"

namespace po = boost::program_options;

int main(int argc, const char *argv[]) {

    try {
        scan::filenames_t included, excluded;
        size_t level;

        po::options_description desc{"Options"};
        desc.add_options()
                ("help,h", "Suppported commands")
                ("incdir,i", po::value<scan::filenames_t>(&included)->composing(), "directories for scan")
                ("excdir,e", po::value<scan::filenames_t>(&excluded)->composing(), "excluded directories for scan")
                ("level,l", po::value<size_t>(&level)->composing(), "scan level: 1 - recursive, 0 - current directory only")
                ("minsize,s", po::value<size_t>()->default_value(1), "minimal size of file")
                ("mask,m", po::value<scan::mask_t>()->composing(), "masks of files")
                ("blocksize,b", po::value<size_t>()->default_value(1), "compare block size")
                ("hashtype,t", po::value<size_t>()->default_value(0), "hash type: 0 - md5, 1 - crc32")
                ;

        po::variables_map vm;
        po::store(parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")) {
            std::cout << desc << '\n';
        }

        po::notify(vm);
        scan::printFileList(scan::fileList(included, excluded, static_cast<scan::level_t>(level)));

//        filelist::FileListCreator fl(dirs);

//        if (vm.count("help")) {
//            std::cout << desc << '\n';
//        } else if (vm.count("incdir")) {
//            std::cout << "readfrom: " << vm["config"].as<std::string>() << std::endl;
//        } else if (vm.count("bulk")) {
//            std::cout << "bulk: " << vm["bulk"].as<size_t>() << std::endl;
//        } else if (vm.count("bulk")) {
//            std::cout << "bulk: " << vm["bulk"].as<size_t>() << std::endl;
//        } else if (vm.count("bulk")) {
//            std::cout << "bulk: " << vm["bulk"].as<size_t>() << std::endl;
//        } else if (vm.count("bulk")) {
//            std::cout << "bulk: " << vm["bulk"].as<size_t>() << std::endl;
//        } else if (vm.count("bulk")) {
//            std::cout << "bulk: " << vm["bulk"].as<size_t>() << std::endl;
//        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

}
