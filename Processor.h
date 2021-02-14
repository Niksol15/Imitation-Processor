//
// Created by solon on 14.02.2021.
//

#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "Register.h"

class Processor {
public:
    Processor();

    enum class Operator;

    Processor::Operator ProcessCommand(std::string command) noexcept;

    friend std::ostream &operator<<(std::ostream &os, const Processor &proc);

private:
    void MoveTo(const std::string &operand);

    void MoveFrom(const std::string &operand);

    void Shift(const std::string &operand);

    void Add(const std::string &operand);

    Register accumulator;
    Register r1;
    Register r2;
    Register r3;
    std::string curr_command;
    int PC;
    int TC;
    char PS;
};

#endif //PROCESSOR_H
