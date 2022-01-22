#define BOOST_TEST_MODULE tests

#include <boost/test/unit_test.hpp>

#include "filesfilter.h"
namespace ff = filesfilter;
namespace def = ff::def;

BOOST_AUTO_TEST_SUITE(SameFilesFinder)

BOOST_AUTO_TEST_CASE(filelist_included)
{
    ff::filenames_t included = def::Included;
    ff::filenames_t excluded = def::Excluded;
    ff::mask_t mask = def::Mask;
    size_t level = def::Level;
    size_t minsize = def::Minsize;
    size_t blocksize = def::Blocksize;
    size_t hashtype = def::Alg;
    included = {
        "../otus_homework_07/_test_folder"
    };


    ff::SameFilesFinder scanner = ff::SameFilesFinder(
                included, excluded, mask, static_cast<ff::level_t>(level),
                minsize, blocksize, static_cast<hash::alg_t>(hashtype)
                );

    BOOST_CHECK(scanner.fileList().size() == 5);

    included = {
        "../otus_homework_07/_test_folder/dir1",
        "../otus_homework_07/_test_folder/dir2"
    };

    scanner = ff::SameFilesFinder(
                    included, excluded, mask, static_cast<ff::level_t>(level),
                    minsize, blocksize, static_cast<hash::alg_t>(hashtype)
                    );

    BOOST_CHECK(scanner.fileList().size() == 7);
}

BOOST_AUTO_TEST_CASE(filelist_excluded)
{
    ff::filenames_t included = def::Included;
    ff::filenames_t excluded = def::Excluded;
    ff::mask_t mask = def::Mask;
    size_t level = def::Level;
    size_t minsize = def::Minsize;
    size_t blocksize = def::Blocksize;
    size_t hashtype = def::Alg;
    included = {
        "../otus_homework_07/_test_folder/dir1",
        "../otus_homework_07/_test_folder/dir2"
    };

    excluded = {
        "../otus_homework_07/_test_folder/dir1"
    };


    ff::SameFilesFinder scanner = ff::SameFilesFinder(
                included, excluded, mask, static_cast<ff::level_t>(level),
                minsize, blocksize, static_cast<hash::alg_t>(hashtype)
                );

    BOOST_CHECK(scanner.fileList().size() == 4);
}

BOOST_AUTO_TEST_CASE(filelist_level)
{
    ff::filenames_t included = def::Included;
    ff::filenames_t excluded = def::Excluded;
    ff::mask_t mask = def::Mask;
    size_t level = def::Level;
    size_t minsize = def::Minsize;
    size_t blocksize = def::Blocksize;
    size_t hashtype = def::Alg;

    included = {
        "../otus_homework_07/_test_folder"
    };

    level = ff::All;

    ff::SameFilesFinder scanner = ff::SameFilesFinder(
                included, excluded, mask, static_cast<ff::level_t>(level),
                minsize, blocksize, static_cast<hash::alg_t>(hashtype)
                );

    BOOST_CHECK_EQUAL(scanner.fileList().size(), 16);
}

BOOST_AUTO_TEST_CASE(filelist_mask)
{
    ff::filenames_t included = def::Included;
    ff::filenames_t excluded = def::Excluded;
    ff::mask_t mask = def::Mask;
    size_t level = def::Level;
    size_t minsize = def::Minsize;
    size_t blocksize = def::Blocksize;
    size_t hashtype = def::Alg;

    included = {
        "../otus_homework_07/_test_folder"
    };

    level = ff::All;

    mask = "non.*";

    ff::SameFilesFinder scanner = ff::SameFilesFinder(
                included, excluded, mask, static_cast<ff::level_t>(level),
                minsize, blocksize, static_cast<hash::alg_t>(hashtype)
                );
//    scanner.printFileList();
    BOOST_CHECK_EQUAL(scanner.fileList().size(), 8);

    mask = "dub.*";
    scanner = ff::SameFilesFinder(
                    included, excluded, mask, static_cast<ff::level_t>(level),
                    minsize, blocksize, static_cast<hash::alg_t>(hashtype)
                    );
    BOOST_CHECK_EQUAL(scanner.fileList().size(), 8);

    mask = ".*dub.*";
    scanner = ff::SameFilesFinder(
                    included, excluded, mask, static_cast<ff::level_t>(level),
                    minsize, blocksize, static_cast<hash::alg_t>(hashtype)
                    );
    BOOST_CHECK_EQUAL(scanner.fileList().size(), 16);

    mask = ".*dub.*_3_.*";
    scanner = ff::SameFilesFinder(
                    included, excluded, mask, static_cast<ff::level_t>(level),
                    minsize, blocksize, static_cast<hash::alg_t>(hashtype)
                    );
    BOOST_CHECK_EQUAL(scanner.fileList().size(), 4);

    mask = ".*txt";
    scanner = ff::SameFilesFinder(
                    included, excluded, mask, static_cast<ff::level_t>(level),
                    minsize, blocksize, static_cast<hash::alg_t>(hashtype)
                    );
    BOOST_CHECK_EQUAL(scanner.fileList().size(), 16);

    mask = ".*tt";
    scanner = ff::SameFilesFinder(
                    included, excluded, mask, static_cast<ff::level_t>(level),
                    minsize, blocksize, static_cast<hash::alg_t>(hashtype)
                    );
    BOOST_CHECK_EQUAL(scanner.fileList().size(), 0);

    mask = ".*tt.*";
    scanner = ff::SameFilesFinder(
                    included, excluded, mask, static_cast<ff::level_t>(level),
                    minsize, blocksize, static_cast<hash::alg_t>(hashtype)
                    );
    BOOST_CHECK_EQUAL(scanner.fileList().size(), 0);

    mask = "tt";
    scanner = ff::SameFilesFinder(
                    included, excluded, mask, static_cast<ff::level_t>(level),
                    minsize, blocksize, static_cast<hash::alg_t>(hashtype)
                    );
    BOOST_CHECK_EQUAL(scanner.fileList().size(), 0);

}

BOOST_AUTO_TEST_CASE(filelist_minsize)
{
    ff::filenames_t included = def::Included;
    ff::filenames_t excluded = def::Excluded;
    ff::mask_t mask = def::Mask;
    size_t level = def::Level;
    size_t minsize = def::Minsize;
    size_t blocksize = def::Blocksize;
    size_t hashtype = def::Alg;

    included = {
        "../otus_homework_07/_test_folder"
    };

    minsize = 250;

    ff::SameFilesFinder scanner = ff::SameFilesFinder(
                included, excluded, mask, static_cast<ff::level_t>(level),
                minsize, blocksize, static_cast<hash::alg_t>(hashtype)
                );

    BOOST_CHECK_EQUAL(scanner.fileList().size(), 2);
}

BOOST_AUTO_TEST_CASE(samefiles)
{
    ff::filenames_t included = def::Included;
    ff::filenames_t excluded = def::Excluded;
    ff::mask_t mask = def::Mask;
    size_t level = def::Level;
    size_t minsize = def::Minsize;
    size_t blocksize = def::Blocksize;
    size_t hashtype = def::Alg;

    included = {
        "../otus_homework_07/_test_folder"
    };
    level = ff::All;

    ff::SameFilesFinder scanner = ff::SameFilesFinder(
                included, excluded, mask, static_cast<ff::level_t>(level),
                minsize, blocksize, static_cast<hash::alg_t>(hashtype)
                );

    BOOST_CHECK_EQUAL(scanner.sameFilesGroups().size(), 3);

    scanner.printSameFiles();
}

}

