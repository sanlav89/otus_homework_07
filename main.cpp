#include <boost/program_options.hpp>
#include <iostream>
#include <list>

#include "filelist.h"

namespace po = boost::program_options;

using strlist_t = std::vector<std::string>;

int main(int argc, const char *argv[]) {
    try {
        filelist::dirs_t dirs;
        po::options_description desc{"Options"};
        desc.add_options()
                ("help,h", "Suppported commands")
                ("incdir,i", po::value<strlist_t>(&dirs)->composing()->required(), "directories for scan")
                ("excdir,e", po::value<strlist_t>()->composing(), "excluded directories for scan")
                ("minsize,s", po::value<size_t>()->default_value(1), "minimal size of file")
                ("mask,m", po::value<strlist_t>()->composing(), "masks of files")
                ("blocksize,b", po::value<size_t>()->default_value(1), "compare block size")
                ("hashtype,t", po::value<size_t>()->default_value(0), "hash type: 0 - md5, 1 - crc32")
                ;

        po::variables_map vm;
        po::store(parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")) {
            std::cout << desc << '\n';
        }

        po::notify(vm);

        filelist::FileListCreator fl(dirs);

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
//        }*/
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
