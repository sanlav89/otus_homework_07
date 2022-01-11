#define BOOST_TEST_MODULE tests

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(tests)

BOOST_AUTO_TEST_CASE(test_case_1)
{
    BOOST_CHECK(1 > 0);
}

BOOST_AUTO_TEST_CASE(test_case_2)
{
    BOOST_CHECK(1 > 0);
}

}

