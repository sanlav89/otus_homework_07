#include <boost/program_options.hpp>
#include <iostream>
#include <list>
#include <set>
#include <algorithm>
#include "filesfilter.h"

namespace po = boost::program_options;
namespace ff = filesfilter;

using boost::uuids::detail::md5;

std::string toString(const md5::digest_type &digest)
{
    const auto charDigest = reinterpret_cast<const char *>(&digest);
    std::string result;
    boost::algorithm::hex(charDigest, charDigest + sizeof(md5::digest_type), std::back_inserter(result));
    return result;
}

int main(int argc, const char *argv[])
{

    try {

        std::string ddd("qwer");
        md5 hash, hash2;
        md5::digest_type digest, digest2;

        hash.process_bytes(ddd.data(), ddd.size());
        hash.get_digest(digest);

        hash2.process_bytes("qwer", 4);
        hash2.get_digest(digest2);

        boost::crc_32_type crc32;
        crc32.process_bytes(ddd.data(), ddd.size());

        std::cout << digest << " " << toString(digest) << std::endl;
        std::cout << digest2 << " " << toString(digest2) << std::endl;

        std::cout << hash::crc32_t(ddd.data(), ddd.size()).toString() << std::endl;
        std::cout << hash::md5_t(ddd.data(), ddd.size()).toString() << std::endl;

//        printf("%08X%08X%08X%08X\n", digest[0], digest[1], digest[2], digest[3]);
//        printf("%08X\n", crc32.checksum());

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
