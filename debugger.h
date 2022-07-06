#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <QWidget>
#include <QListWidget>
#include "processor.h"


class Debugger
{
private:
    enum OP_CODES{
        NOP = 0x0000,
        STOP = 0x00FF,
        MOV = 0x0001,
        ADD = 0x0002,
        SUB = 0x0003,
        CMP = 0x0004,
        MUL = 0x0005,
        DIV = 0x0006,

        AND = 0x0007,
        OR = 0x0008,
        XOR = 0x0009,
        NOT = 0x001E,

        PUSH = 0x002E,
        POP = 0x003E,

        SHL = 0x004E,
        SHR = 0x005E,
        SAR = 0x006E,
        ROL = 0x007E,
        ROR = 0x008E,
        RCL = 0x009E,
        RCR = 0x00AE,

        JMP = 0x001C,
        JGE = 0x002C,
        JL = 0x003C,
        JNZ = 0x004C,
        JZ = 0x005C,
        JLE = 0x006C,
        JG = 0x007C,



        CALL = 0x009D,
        RET = 0x000D,

        PIXEL = 0x009c


    };
    enum class MOV_TYPES{
        REGISTER_TO_REGISTER = 0,
        NUMBER_TO_REGISTER = 1,
        ADRESS_TO_REGISTER = 2,
        REGISTER_TO_ADRESS = 3,
        NUMBER_TO_ADRESS = 4,
        ADRESS_TO_ADRESS = 5,
//        REGISTER_TO_ADRESS_PLUS = 6,
//        NUMBER_TO_ADRESS_PLUS = 7,
//        ADRESS_TO_ADRESS_PLUS = 8,
        UNDEFINED = -1
    };

    enum class MOV_TYPES_S{
        REGISTER = 0,
        ADRESS = 1,
        UNDEFINED = -1
    };

public:
    Debugger();

    void update(Processor* CPU, QListWidget* LIST);
    int getOPType(unsigned char OP1, unsigned char OP2);
    int getOPType(unsigned char OP);
};

#endif // DEBUGGER_H
