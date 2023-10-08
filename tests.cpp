#include "catch.hpp"
#include "BigInt.cpp"

TEST_CASE("BigInt arithmetic operations") {
BigInt a("1234567890");
BigInt b("-1234567890");
BigInt c("2469135780");
BigInt d("0");

SECTION("Addition") {
REQUIRE(a + a == c);
REQUIRE(a + b == d);
}

SECTION("Subtraction") {
REQUIRE(a - a == d);
REQUIRE(a - b == c);
}

SECTION("Multiplication") {
REQUIRE(a * b == -c * c);
REQUIRE(b * b == c * c);
}

SECTION("Division") {
REQUIRE_THROWS_AS(a / d, std::invalid_argument);
REQUIRE(c / a == BigInt("2"));
}

SECTION("Modulo") {
REQUIRE(c % a == d);
}
}
