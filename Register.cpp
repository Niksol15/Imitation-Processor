//
// Created by solon on 14.02.2021.
//

#include <sstream>
#include <cmath>
#include "Register.h"

using namespace std;

bool IsNum(const string &str) {
    istringstream is(str);
    try {
        int num;
        is >> num;
        return true;
    } catch (exception &) {
        return false;
    }
}

int StrToNum(const string &str) {
    istringstream is(str);
    int num;
    is >> num;
    return num;
}

namespace {
    Bits toDirectCode(int num) {
        Bits res{};
        if (num < 0) {
            res[0] = true;
            num *= -1;
        }
        for (int i = REG_SIZE - 1; i > 0; --i) {
            res[i] = num % 2;
            num /= 2;
        }
        return res;
    }

    Bits toComplementCode(Bits code) {
        if (code[0]) {
            for (bool &bit: code) {
                bit = !bit;
            }
            code[0] = true;
            Bits sup_one = REG_ONE;
            for (int i = REG_SIZE - 1; i >= 0; --i) {
                if (code[i] && sup_one[i]) {
                    code[i] = false;
                    if (i > 0) sup_one[i - 1] = true;
                } else if (sup_one[i]) {
                    code[i] = true;
                }
            }
        }
        return code;
    }
}

Register::Register() noexcept: bits{}, overflow{false}, sign{'+'} {

}

Register &Register::operator=(int num) noexcept {
    if (num < 0) {
        sign = '-';
    }
    if (num > pow(2, REG_SIZE - 1) || num < -pow(2, REG_SIZE - 1)) {
        overflow = true;
    }
    bits = toComplementCode(toDirectCode(num));
    return *this;
}

Register &Register::operator+=(const Register &other) noexcept {
    return *this += other.bits;
}

Register &Register::operator+=(int num) noexcept {
    return *this += toComplementCode(toDirectCode(num));
}

Register &Register::operator+=(const Bits &other) noexcept {
    overflow = false;
    for (int i = REG_SIZE - 1; i >= 0; --i) {
        if (bits[i] && other[i]) {
            bits[i] = overflow;
            overflow = true;
        } else if (!other[i] && !bits[i]) {
            bits[i] = overflow;
            overflow = false;
        } else {
            bits[i] = !overflow;
        }
    }
    return *this;
}

char Register::GetSign() const noexcept {
    return sign;
}

void Register::Shift(int num) noexcept {
    if (num < 0) {
        num *= -1;
        num %= REG_SIZE;
        ShiftRight(REG_SIZE - num);
    } else {
        num %= REG_SIZE;
        ShiftRight(num);
    }
}

void Register::ShiftRight(int shift) noexcept {
    Bits temp{};
    for (int i = 0; i < REG_SIZE; ++i) {
        temp[(i + shift) % REG_SIZE] = bits[i];
    }
    bits = temp;
}

ostream &operator<<(ostream &os, const Register &n) {
    for (bool bit: n.bits) {
        os << bit << ' ';
    }
    return os;
}