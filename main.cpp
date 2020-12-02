#include <iostream>
#include <fstream>
#include <array>
#include <cmath>
#include <utility>
#include <string>
#include <sstream>

using namespace std;

constexpr int REG_SIZE = 18;

using Bits = array<bool, REG_SIZE>;

//NOLINTNEXTLINE
const Bits REG_ONE = [](){Bits res{}; res[REG_SIZE - 1] = true; return res;}();

bool IsNum(const string& str){
    istringstream is(str);
    try{
        int num;
        is >> num;
        return true;
    } catch (exception&) {
        return false;
    }
}
int StrToNum(const string& str){
    istringstream is(str);
    int num;
    is >> num;
    return num;
}

class Register{
public:
    Register& operator=(int num){
        if(num < 0){
            sign = '-';
        }
        if(num > pow(2, 17) || num < -pow(2, 17)){
            overflow = true;
        }
        bits = toComplementCode(toDirectCode(num));
        return *this;
    }
    Register& operator=(const Register& other) = default;
    Register& operator+=(const Register& other){
        return *this += other.bits;
    }
    Register& operator+=(int num){
        return *this += toComplementCode(toDirectCode(num));
    }
    Register& operator+=(const Bits& other){
        overflow = false;
        for(int i = REG_SIZE - 1; i >= 0; --i){
            if(bits[i] && other[i]){
                bits[i] = overflow;
                overflow = true;
            } else if(!other[i] && !bits[i]){
                bits[i] = overflow;
                overflow = false;
            } else{
                bits[i] = !overflow;
            }
        }
        return *this;
    }
    [[nodiscard]] char GetSign() const {
        return sign;
    }
    void Shift(int num){
        if(num < 0){
            num *= -1;
            num %= REG_SIZE;
            ShiftRight(REG_SIZE - num);
        } else {
            num %= REG_SIZE;
            ShiftRight(num);
        }
    }
    friend ostream& operator<<(ostream& os, const Register& n);
private:
    static Bits toDirectCode(int num){
        Bits res{};
        if(num < 0){
            res[0] = true;
            num *= -1;
        }
        for(int i = REG_SIZE - 1; i > 0; --i){
            res[i] = num % 2;
            num /= 2;
        }
        return  res;
    }
    static Bits toComplementCode(Bits code){
        if(code[0]){
            for(bool& bit: code){
                bit = !bit;
            }
            code[0] = true;
            Bits sup_one = REG_ONE;
            for(int i = REG_SIZE - 1; i >= 0; --i){
                if(code[i] && sup_one[i]){
                    code[i] = false;
                    if(i > 0) sup_one[i - 1] = true;
                } else if(sup_one[i]){
                    code[i] = true;
                }
            }
        }
        return code;
    }
    void ShiftRight(int shift){
        Bits temp{};
        for(int i = 0; i < REG_SIZE; ++i){
            temp[(i + shift)%REG_SIZE] = bits[i];
        }
        bits = temp;
    }
    Bits bits = {};
    bool overflow = false;
    char sign = '+';
};

ostream& operator<<(ostream& os, const Register& n){
    for(bool bit: n.bits){
        os << bit << ' ';
    }
    return os;
}

enum class Operator{
    MOVETO,
    MOVEFROM,
    ADD,
    SHIFT,
    UNDEFINED
};

class Processor{
public:
    void ProcessCommand(string command){
        ++TC;
        istringstream is(command);
        curr_command = move(command);
        string command_operator_str;
        is >> command_operator_str;
        string operand;
        is >> operand;
        Operator comm_operator = ParseOperator(command_operator_str);
        cout << *this ;
        cin.get();
        switch(comm_operator){
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
    }
    friend ostream& operator<<(ostream& os, const Processor& proc);
private:

    void MoveTo(const string& operand){
        if(operand == "R1"){
            accumulator = r1;
        } else if(operand == "R2"){
            accumulator = r2;
        } else if(operand == "R3"){
            accumulator = r3;
        } else if(IsNum(operand)){
            accumulator = StrToNum(operand);
        }
    }
    void MoveFrom(const string& operand){
        if(operand == "R1"){
            r1 = accumulator;
        } else if(operand == "R2"){
            r2 = accumulator;
        } else if(operand == "R3"){
            r3 = accumulator;
        }
    }
    void Shift(const string& operand){
        if(IsNum(operand)){
            int value = StrToNum(operand);
            accumulator.Shift(value);
        }
    }
    void Add(const string& operand){
        if(operand == "R1"){
            accumulator += r1;
        } else if(operand == "R2"){
            accumulator += r2;
        } else if(operand == "R3"){
            accumulator += r3;
        } else if(IsNum(operand)){
            accumulator += StrToNum(operand);
        }
    }
    static Operator ParseOperator(const string& command) {
        if (command == "moveto") {
            return Operator::MOVETO;
        } else if (command == "movefrom") {
            return Operator::MOVEFROM;
        } else if (command == "add") {
            return Operator::ADD;
        } else if (command == "shift") {
            return Operator::SHIFT;
        } else {
            return Operator::UNDEFINED;
        }
    }
    Register accumulator;
    Register r1;
    Register r2;
    Register r3;
    string curr_command{};
    int PC = 0;
    int TC = 0;
    char PS = '+';
};

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

//NOLINTNEXTLINE
const string FILE_PATH = "Commands.txt";

int main() {
    Processor proc;
    ifstream fin(FILE_PATH);
    string command{};
    while (getline(fin, command)){
        cin.get();
        proc.ProcessCommand(command);
    }
    return 0;
}
