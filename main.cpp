#include <boost/program_options.hpp>
#include "filesfilter.h"

namespace po = boost::program_options;
namespace ff = filesfilter;
namespace def = ff::def;

// Windows run example
// D:\WORK\OTUS\build-otus_homework_07-Desktop_Qt_5_15_0_MinGW_64_bit-Debug>bayan -i ../otus_homework_07/_test_folder/ -l 1
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
                ("level,l", po::value<size_t>(&level)->default_value(def::Level), "scan level: 1 - recursive, 0 - current directory only")
                ("mask,m", po::value<ff::mask_t>(&mask)->composing()->default_value(def::Mask), "regex: masks of files")
                ("minsize,s", po::value<size_t>(&minsize)->default_value(def::Minsize), "minimal size of file")
                ("blocksize,b", po::value<size_t>(&blocksize)->default_value(def::Blocksize), "compare block size")
                ("hashtype,t", po::value<size_t>(&hashtype)->default_value(def::Alg), "hash type: 0 - md5, 1 - crc32")
                ;

        po::variables_map vm;
        po::store(parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")) {
            std::cout << desc << '\n';
        } else {

            po::notify(vm);

            if (included.empty()) {
                included = def::Included;
            }

            ff::SameFilesFinder scanner(
                        included, excluded, mask, static_cast<ff::level_t>(level),
                        minsize, blocksize, static_cast<hash::alg_t>(hashtype)
                        );

//            scanner.printFileList();
            scanner.printSameFiles();

        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

}
