#ifndef PROCESSOR_H
#define PROCESSOR_H

#define RAM_SIZE 128
#define REG_COUNT 4
#include <QObject>



class Processor: public QObject
{
    Q_OBJECT
    //    unsigned short RAM[RAM_SIZE];
    //    unsigned short REG[REG_COUNT];
    //    unsigned short PC = 0;
    //    unsigned short IR = 0;

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

        JMP = 0x001C,
        JGE = 0x002C,
        JL = 0x003C,
        JNZ = 0x004C,
        JZ = 0x005C,
        JLE = 0x006C,
        JG = 0x007C,

        PUSH = 0x002E,
        POP = 0x003E,
        SHL = 0x004E,
        SHR = 0x005E,
        ROL = 0x007E,
        ROR = 0x008E,

        CALL = 0x009D,
        RET = 0x000D
    };


    enum class MOV_TYPES{
        REGISTER_TO_REGISTER = 0,
        NUMBER_TO_REGISTER = 1,
        ADRESS_TO_REGISTER = 2,
        REGISTER_TO_ADRESS = 3,
        NUMBER_TO_ADRESS = 4,
        ADRESS_TO_ADRESS = 5,
        UNDEFINED = -1
    };

public:
    Processor();
public:
    unsigned short RAM[RAM_SIZE];
    unsigned short REG[REG_COUNT];
    unsigned short PC = 0;
    unsigned short IR = 0;
    unsigned short PS = 0;

    bool isRunning =  false;


    unsigned short processor_readRAM();
    void loop();
    void step(bool *isRunning);
    void writeRAM(unsigned short * DATA);
    void reset();
    void resetCommand();
    unsigned char getOPType(unsigned char OP1, unsigned char OP2);

signals:
    void dataChanged();
    void evokeException(QString msg);

public slots:
    void onStep();

};





#endif // PROCESSOR_H
