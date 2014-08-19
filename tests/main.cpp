#define BOOST_TEST_MODULE jajo
#define BOOST_TEST_MAIN 1
#include <boost/test/unit_test.hpp>

#include "example_reflected_lib/enums.hpp"
#include "example_reflected_lib/enums_ref.hxx"

BOOST_AUTO_TEST_CASE(aritmetic)
{
	BOOST_REQUIRE_EQUAL(4, 2*2);
}
