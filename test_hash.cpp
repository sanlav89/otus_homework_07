#define BOOST_TEST_MODULE tests

#include <boost/test/unit_test.hpp>

#include "hash.h"

BOOST_AUTO_TEST_SUITE(Hash)

BOOST_AUTO_TEST_CASE(md5)
{
    auto valueMd5 = hash::Hash("qwer", 4, hash::AlgMd5); // D0122096D970819B9F66F012079D7D6D
    auto valueMd5_2 = hash::Hash("qwer2", 5, hash::AlgMd5); // 335F9A782C917739A34CDEC582840CE1

    BOOST_CHECK(valueMd5_2 < valueMd5);

    BOOST_CHECK_EQUAL(valueMd5.toString(), "D0122096D970819B9F66F012079D7D6D");

    BOOST_CHECK_EQUAL(valueMd5.toString().size(), 32);
}

BOOST_AUTO_TEST_CASE(crc32)
{
    auto valueCrc32 = hash::Hash("qwer", 4, hash::AlgCrc32).toString();

    BOOST_CHECK_EQUAL(valueCrc32, "C2E9F805");

    BOOST_CHECK_EQUAL(valueCrc32.size(), 8);
}

}
