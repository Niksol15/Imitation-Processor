//
// Created by solon on 14.02.2021.
//

#ifndef REGISTER_H
#define REGISTER_H

#include <iostream>
#include <string>
#include <array>

bool IsNum(const std::string &str);

int StrToNum(const std::string &str);

constexpr int REG_SIZE = 18;

using Bits = std::array<bool, REG_SIZE>;

//NOLINTNEXTLINE
const Bits REG_ONE = []() {
    Bits res{};
    res[REG_SIZE - 1] = true;
    return res;
}();

class Register {
public:
    Register() noexcept;

    Register &operator=(int num) noexcept;

    Register &operator=(const Register &other) noexcept = default;

    Register &operator+=(const Register &other) noexcept;

    Register &operator+=(int num) noexcept;

    Register &operator+=(const Bits &other) noexcept;

    [[nodiscard]] char GetSign() const noexcept;

    void Shift(int num) noexcept;

    friend std::ostream &operator<<(std::ostream &os, const Register &n);

private:
    void ShiftRight(int shift) noexcept;

    Bits bits;
    bool overflow;
    char sign;
};


#endif //REGISTER_H
