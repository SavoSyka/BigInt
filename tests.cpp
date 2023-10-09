#include "catch.hpp"
#include "BigInt.hpp"

TEST_CASE("BigInt arithmetic operations") {
    BigInt a("123456780");
    BigInt b("-123456780");
    BigInt c("246913560");
    BigInt d("0");

    SECTION("Addition") {
        REQUIRE(a + a == c);
        REQUIRE(a + b == d);
        REQUIRE(a + 0 == a);
        REQUIRE(++a == a+1);

    }

    SECTION("Subtraction") {
        REQUIRE(a - a == d);
        REQUIRE(a - b == c);
        REQUIRE(a - 0 == a);
        REQUIRE(--a == a-1);
    }

    SECTION("Multiplication") {
        REQUIRE(a * b * 4 == -c * c);
        REQUIRE(b * b * 4 == c * c);
    }

    SECTION("Division") {
        REQUIRE_THROWS_AS(a / d, std::invalid_argument);
        REQUIRE(c / a == BigInt("2"));
    }

    SECTION("Modulo") {
        REQUIRE(c % a == d);
    }
}
