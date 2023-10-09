#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <cmath>

using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using BigIntBaseType = uint16_t;
const int base =  static_cast<int>(std::log10(std::numeric_limits<BigIntBaseType>::max()));

class BigInt{
public:
    bool sign;
    std::vector<u16> digits;

public:
    BigInt() : sign(true) {}
    BigInt(const std::string& str) {
        sign = (str[0] == '-');
        int start = sign ? 1 : 0;
        for (size_t i = start; i < str.length(); ++i) {
            u16 carry = str[i] - '0';
            for (auto& digit : digits) {
                u32 tmp = static_cast<u32>(digit) * 10 + carry;
                digit = tmp & 0xFFFF;
                carry = tmp >> 16;
            }
            if (carry > 0) digits.push_back(carry);
        }
    }

    BigInt operator-() const {
        BigInt result = *this;
        result.sign = !result.sign;
        return result;
    }

    bool operator<(const BigInt& other) const {
        if (sign != other.sign) return sign;
        if (digits.size() != other.digits.size())
            return digits.size() < other.digits.size();
        for (int i = digits.size() - 1; i >= 0; --i) {
            if (digits[i] != other.digits[i])
                return digits[i] < other.digits[i];
        }
        return false;
    }

    bool operator>(const BigInt& other) const {
        return !(*this < other) && *this != other;
    }

    bool operator<=(const BigInt& other) const {
        return !(*this > other);
    }

    bool operator>=(const BigInt& other) const {
        return !(*this < other);
    }

    bool operator==(const BigInt& other) const {
        return sign == other.sign && digits == other.digits;
    }

    bool operator!=(const BigInt& other) const {
        return !(*this == other);
    }

    BigInt operator+(const BigInt& other) const {
        if (sign != other.sign) {
            if (sign) return other - (-*this);
            else return *this - (-other);
        }
        BigInt result;
        result.sign = sign;
        result.digits.resize(std::max(digits.size(), other.digits.size()));
        u32 carry = 0;
        for (size_t i = 0; i < result.digits.size(); ++i) {
            u32 a = (i < digits.size()) ? digits[i] : 0;
            u32 b = (i < other.digits.size()) ? other.digits[i] : 0;
            u32 tmp = a + b + carry;
            result.digits[i] = tmp & 0xFFFF;
            carry = tmp >> 16;
        }
        if (carry > 0) result.digits.push_back(carry);
        if (result == BigInt("-0")){
            result.sign = 0;
        }
        return result;
    }

    BigInt operator-(const BigInt& other) const {
        if (sign != other.sign) return *this + (-other);
        if ((sign && *this < other) || (!sign && *this > other)) return -(other - *this);
        BigInt result;
        result.sign = sign;
        result.digits.resize(std::max(digits.size(), other.digits.size()));
        int32_t carry = 0;
        for (size_t i = 0; i < result.digits.size(); ++i) {
            int32_t a = (i < digits.size()) ? digits[i] : 0;
            int32_t b = (i < other.digits.size()) ? other.digits[i] : 0;
            int32_t tmp = a - b - carry;
            if (tmp < 0) {
                tmp += 65536;
                carry = 1;
            } else {
                carry = 0;
            }
            result.digits[i] = tmp & 0xFFFF;
        }
        while (!result.digits.empty() && result.digits.back() == 0) {
            result.digits.pop_back();
            if (result.digits.empty()) {
                result.sign = true;
            }
        }
        if (carry > 0) {
            result.sign = !result.sign;
            for (auto& digit : result.digits) {
                digit = ~digit & 0xFFFF;
            }
            carry = 1;
            for (auto& digit : result.digits) {
                u32 tmp = static_cast<u32>(digit) + carry;
                digit = tmp & 0xFFFF;
                carry = tmp >> 16;
            }
            if (carry > 0) result.digits.push_back(carry);
        }
        if (result == BigInt("-0")){
            result.sign = 0;
        }
        return result;
    }

    BigInt operator+(const int other){
        return *this + BigInt(std::to_string(other));
    }
    BigInt operator-(const int other){
        return *this - BigInt(std::to_string(other));
    }

    BigInt& operator+=(const BigInt& other) {
        *this = *this + other;
        return *this;
    }
    BigInt& operator+=(const int& other) {
        *this = *this + other;
        return *this;
    }

    BigInt& operator-=(const BigInt& other) {
        *this = *this - other;
        return *this;
    }
    BigInt& operator-=(const int& other) {
        *this = *this - other;
        return *this;
    }

    BigInt& operator++() {
        *this += BigInt("1");
        return *this;
    }
    BigInt operator++(int) {
        BigInt temp = *this;
        ++*this;
        return temp;
    }

    BigInt& operator--() {
        *this -= BigInt("1");
        return *this;
    }
    BigInt operator--(int) {
        BigInt temp = *this;
        --*this;
        return temp;
    }

    BigInt operator%(const BigInt& other) const {
        BigInt dividend = *this;
        BigInt divisor = other;
        BigInt remainder = dividend;
        while (remainder >= divisor) {
            remainder -= divisor;
        }
        remainder.sign = sign;
        return remainder;
    }
    BigInt operator%(const int& other) const {
        return *this % BigInt(std::to_string(other));
    }
    BigInt& operator%=(const BigInt& other) {
        *this = *this % other;
        return *this;
    }
    BigInt& operator%=(const int& other) {
        *this = *this % BigInt(std::to_string(other));
        return *this;
    }

    BigInt operator/(const BigInt& other) const {
        if (other == BigInt("0")) {
            throw std::invalid_argument("Division by zero");
        }
        if (*this < other) return BigInt("0");
        BigInt dividend = *this;
        BigInt divisor = other;
        BigInt quotient("0");
        while (dividend >= divisor) {
            dividend -= divisor;
            ++quotient;
        }
        quotient.sign = sign ^ other.sign;
        return quotient;
    }
    BigInt operator/(const int& other) const {
        return *this / BigInt(std::to_string(other));
    }
    BigInt operator/=(const BigInt& other){
        if (other == BigInt("0")) {
            throw std::invalid_argument("Division by zero");
        }
        *this = *this / other;
        return *this;
    }
    BigInt operator/=(const int& other){
        if (other == 0) {
            throw std::invalid_argument("Division by zero");
        }
        *this = *this / BigInt(std::to_string(other));
        return *this;
    }

    BigInt operator*(const BigInt& other) const {
        BigInt result;
        result.digits.resize(digits.size() + other.digits.size(), 0);
        for (size_t i = 0; i < digits.size(); i++) {
            u64 carry = 0;
            for (size_t j = 0; j < other.digits.size(); j++) {
                u64 temp = static_cast<u64>(digits[i]) * other.digits[j] + result.digits[i+j] + carry;
                result.digits[i+j] = temp % 65536;
                carry = temp / 65536;
            }
            if (carry > 0) {
                result.digits[i + other.digits.size()] += carry;
            }
        }
        while (result.digits.size() > 1 && result.digits.back() == 0)
            result.digits.pop_back();
        result.sign = (sign ^ other.sign);
        if (result == BigInt("-0")){
            result.sign = 0;
        }
        return result;
    }
    BigInt operator*(const int& other) const {
        return *this * BigInt(std::to_string(other));
    }
    BigInt operator*=(const BigInt& other){
        *this = *this * other;
        return *this;
    }

    BigInt operator*=(const int& other){
        *this = *this * BigInt(std::to_string(other));
        return *this;
    }

    BigInt& operator=(const BigInt& other) {
        if (this != &other) {
            digits = other.digits;
            sign = other.sign;
        }
        return *this;
    }

    BigInt& operator=(const int& int_other) {
        BigInt other = BigInt(std::to_string(int_other));
        if (this != &other) {
            digits = other.digits;
            sign = other.sign;
        }
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const BigInt& num) {
        if (num.sign) os << '-';
        u64 result = 0;
        for (int i = num.digits.size() - 1; i >= 0; --i) {
            result += static_cast<u64>(num.digits[i]) * std::pow(65536, i);
        }
        os << result;
        return os;
    }
    friend std::istream& operator>>(std::istream& in, BigInt& num) {
        std::string str;
        in >> str;
        num = BigInt(str);
        return in;
    }
};