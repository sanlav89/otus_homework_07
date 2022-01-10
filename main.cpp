#include <boost/program_options.hpp>
#include <iostream>
#include <list>
#include <set>
#include <algorithm>
#include "filesfilter.h"

namespace po = boost::program_options;
namespace ff = filesfilter;

int main(int argc, const char *argv[])
{

    try {

        ff::filenames_t included, excluded;
        ff::mask_t mask;
        size_t level, minsize, blocksize, hashtype;

        po::options_description desc{"Options"};
        desc.add_options()
                ("help,h", "Suppported commands")
                ("incdir,i", po::value<ff::filenames_t>(&included)->composing(), "directories for scan")
                ("excdir,e", po::value<ff::filenames_t>(&excluded)->composing(), "excluded directories for scan")
                ("level,l", po::value<size_t>(&level)->default_value(ff::Current), "scan level: 1 - recursive, 0 - current directory only")
                ("mask,m", po::value<ff::mask_t>(&mask)->composing()->default_value(".*"), "regex: masks of files")
                ("minsize,s", po::value<size_t>(&minsize)->default_value(1), "minimal size of file")
                ("blocksize,b", po::value<size_t>(&blocksize)->default_value(1), "compare block size")
                ("hashtype,t", po::value<size_t>(&hashtype)->default_value(ff::Md5), "hash type: 0 - md5, 1 - crc32")
                ;

        po::variables_map vm;
        po::store(parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")) {
            std::cout << desc << '\n';
        }

        po::notify(vm);

//        std::cout << minsize << " " << blocksize << " " << hashtype << std::endl;
//        for (const auto &filename : included) {
//            std::cout << filename << std::endl;
//        }

        ff::SameFilesFinder(included, excluded, mask, static_cast<ff::level_t>(level),
                            minsize, blocksize, static_cast<ff::hashtype_t>(hashtype)
                           ).printFileList();

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
