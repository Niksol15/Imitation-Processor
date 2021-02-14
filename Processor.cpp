//
// Created by solon on 14.02.2021.
//

#include <sstream>
#include "Processor.h"

using namespace std;

enum class Processor::Operator {
    MOVETO,
    MOVEFROM,
    ADD,
    SHIFT,
    UNDEFINED
};

namespace {
    Processor::Operator ParseOperator(const string &command) {
        if (command == "moveto") {
            return Processor::Operator::MOVETO;
        } else if (command == "movefrom") {
            return Processor::Operator::MOVEFROM;
        } else if (command == "add") {
            return Processor::Operator::ADD;
        } else if (command == "shift") {
            return Processor::Operator::SHIFT;
        } else {
            return Processor::Operator::UNDEFINED;
        }
    }
}

Processor::Processor() : PC{0}, TC{0}, PS{'+'} {}

Processor::Operator Processor::ProcessCommand(string command) noexcept {
    TC = 0;
    istringstream is(command);
    curr_command = move(command);
    string command_operator_str;
    is >> command_operator_str;
    string operand;
    is >> operand;
    Operator comm_operator = ParseOperator(command_operator_str);
    cout << *this;
    cin.get();
    switch (comm_operator) {
        case Operator::MOVETO:
            MoveTo(operand);
            PS = accumulator.GetSign();
            break;
        case Operator::MOVEFROM:
            MoveFrom(operand);
            PS = accumulator.GetSign();
            break;
        case Operator::ADD:
            Add(operand);
            PS = accumulator.GetSign();
            break;
        case Operator::SHIFT:
            Shift(operand);
            PS = accumulator.GetSign();
            break;
        default:
            break;
    }
    ++TC;
    ++PC;
    cout << *this;
    return comm_operator;
}

void Processor::MoveTo(const string &operand) {
    if (operand == "R1") {
        accumulator = r1;
    } else if (operand == "R2") {
        accumulator = r2;
    } else if (operand == "R3") {
        accumulator = r3;
    } else if (IsNum(operand)) {
        accumulator = StrToNum(operand);
    }
}

void Processor::MoveFrom(const string &operand) {
    if (operand == "R1") {
        r1 = accumulator;
    } else if (operand == "R2") {
        r2 = accumulator;
    } else if (operand == "R3") {
        r3 = accumulator;
    }
}

void Processor::Shift(const string &operand) {
    if (IsNum(operand)) {
        int value = StrToNum(operand);
        accumulator.Shift(value);
    }
}

void Processor::Add(const string &operand) {
    if (operand == "R1") {
        accumulator += r1;
    } else if (operand == "R2") {
        accumulator += r2;
    } else if (operand == "R3") {
        accumulator += r3;
    } else if (IsNum(operand)) {
        accumulator += StrToNum(operand);
    }
}

ostream& operator<<(ostream& os, const Processor& proc){
    os << "IR:          " << proc.curr_command << "\n";
    os << "R1:          " << proc.r1 << "\n";
    os << "R2:          " << proc.r2 << "\n";
    os << "R3:          " << proc.r3 << "\n";
    os << "Accumulator: " << proc.accumulator << "\n";
    os << "PS:          " << proc.PS << "\n";
    os << "PC:          " << proc.PC << "\n";
    os << "TC:          " << proc.TC << "\n";
    os << "----------------------------------";
    os << endl;
    return os;
}