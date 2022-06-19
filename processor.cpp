#include "processor.h"
#include "QDebug"
#include <QApplication>
#include <QElapsedTimer>



Processor::Processor()
{
    reset();

}



void Processor::loop(){
//    isRunning = true;
//    while(isRunning){
//        emit dataChanged();
//        qDebug()<<"PC = "<<PC<<" IR = "<<IR;
//        step(&isRunning);

//        QElapsedTimer timer;
//        timer.start();

//        while ( timer.elapsed() < 1200 )
//            QCoreApplication::processEvents();

//        if(!isRunning || PC >= 64){

//            break;
//        }
//        emit dataChanged();
//    }
}



void Processor::step(bool * isRunning){
    if(*isRunning == false) return;
    IR = PC;
    PC += 2;
    unsigned short OP_CODE = RAM[IR/2];
    switch (OP_CODE & 0x00FF) {

    case NOP:

        qDebug()<<"NOP";
        break;

    case STOP:

        qDebug()<<"STOP";
        PC-=2;
        *isRunning = false;
        break;

    case MOV:{
        qDebug()<<"MOV";

        quint8 OP1 = (OP_CODE & 0xF000)>>12;
        quint8 OP2 = (OP_CODE & 0x0F00)>>8;

        quint8 TYPE = getOPType(OP1, OP2);

        switch (TYPE) {

        case
        (int)MOV_TYPES::
        REGISTER_TO_REGISTER:{

            qDebug()<<"REGISTER TO REGISTER";

            quint16 * REG1_P = &REG[OP1];
            quint16* REG2_P = &REG[OP2];
            *REG2_P = *REG1_P;

            break;
        }

        case
        (int)MOV_TYPES::
        NUMBER_TO_REGISTER:{
            qDebug()<<"NUMBER TO REGISTER";
            quint16* REG2_P = &REG[OP2];
            *REG2_P = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]<<8));
            PC+=2;
            break;
        }

        case
        (int) MOV_TYPES::
        ADRESS_TO_REGISTER:{
            qDebug()<<"ADRESS TO REGISTER";

            quint16* REG1_P = &REG[OP1%4];
            quint16* REG2_P = &REG[OP2];
            if(*REG1_P % 2 == 0){
                quint16 RAM_VAL = RAM[*REG1_P/2]>>8 | ( (RAM[*REG1_P/2]&0x00FF) << 8);

                *REG2_P = RAM_VAL;
            }
            else{
                *REG2_P = RAM[*REG1_P/2]<<8>>8 | ( (RAM[*REG1_P/2+1]) >> 8<<8);
            }

            //*REG2_P = RAM[*REG1_P];
            break;
        }

        case (int) MOV_TYPES::
        REGISTER_TO_ADRESS:{
            qDebug()<<"REGISTER TO ADRESS";

            quint16* REG1_P = &REG[OP1];
            quint16* REG2_P = &REG[OP2%4];
            qDebug()<<*REG1_P<<" "<<*REG2_P;



            if(*REG2_P % 2 == 0){
                RAM[*REG2_P/2] = *REG1_P<<8 | ( (*REG1_P>>8));
            }
            else{
                qDebug()<<*REG2_P/2;
                RAM[*REG2_P/2] = (*REG1_P & 0x00FF);

                RAM[*REG2_P/2 + 1] =(*REG1_P & 0xFF00) ;

            }



            break;
        }

        case (int) MOV_TYPES::
        NUMBER_TO_ADRESS:
        {
            qDebug()<<"NUMBER TO ADRESS";

            quint16 NUMBER = RAM[IR/2+1];
            quint16* REG2_P = &REG[OP2%4];
            qDebug()<<NUMBER<<" "<<*REG2_P;



            if(*REG2_P % 2 == 0){
                RAM[*REG2_P/2] = NUMBER;
            }
            else{
                qDebug()<<*REG2_P/2;
                RAM[*REG2_P/2] = (NUMBER & 0xFF00);

                RAM[*REG2_P/2 + 1] = (NUMBER & 0x00FF);

            }
            PC+=2;


            break;
        }




        case (int) MOV_TYPES::
        ADRESS_TO_ADRESS:{
            qDebug()<<"ADRESS TO ADRESS";

            quint16* REG1_P = &REG[OP1%4];
            quint16* REG2_P = &REG[OP2%4];


            quint16 RAM_VAL = RAM[*REG1_P];

            if(*REG2_P % 2 == 0){
                RAM[*REG2_P/2] = RAM_VAL;
            }
            else{
                qDebug()<<*REG2_P/2;
                RAM[*REG2_P/2] = (RAM_VAL >> 8);

                RAM[*REG2_P/2 + 1] = (RAM_VAL & 0x00FF)<<8;

            }


            break;
        }



        }
        break;
    }


    case ADD:{
        qDebug()<<"ADD";

        quint8 OP1 = (OP_CODE & 0xF000)>>12;
        quint8 OP2 = (OP_CODE & 0x0F00)>>8;

        quint8 TYPE = getOPType(OP1, OP2);
        switch (TYPE) {

        case (int) MOV_TYPES::
        REGISTER_TO_REGISTER:{
            quint16* REG1_P = &REG[OP1];
            quint16* REG2_P = &REG[OP2];
            quint32 REG_SUM = *REG1_P + *REG2_P;
            quint16 REG_SUM_FINAL = REG_SUM & 0xFFFF;
            if(REG_SUM_FINAL != REG_SUM){
                PS = PS | 0b00000000000001;
            }

            *REG2_P = REG_SUM_FINAL;
            break;
        }

        case (int) MOV_TYPES::
        NUMBER_TO_REGISTER:{
            quint16* REG2_P = &REG[OP2];
            quint16 NUMBER = RAM[IR/2+1];
            quint32 REG_SUM = NUMBER + *REG2_P;
            quint16 REG_SUM_FINAL = REG_SUM & 0xFFFF;
            if(REG_SUM_FINAL != REG_SUM){
                PS = PS | 0b00000000000001;
            }

            *REG2_P = REG_SUM_FINAL;
            PC+=2;
            //IR = PC;
            break;
        }


        case (int) MOV_TYPES::
        ADRESS_TO_REGISTER:{
            qDebug()<<"ADRESS TO REGISTER";

            quint16* REG1_P = &REG[OP1%4];
            quint16* REG2_P = &REG[OP2];
            quint16 RAM_VAL;
            if(*REG1_P % 2 == 0){
                RAM_VAL = RAM[*REG1_P/2]>>8 | ( (RAM[*REG1_P/2]&0x00FF) << 8);


            }
            else{
                RAM_VAL = RAM[*REG1_P/2]<<8>>8 | ( (RAM[*REG1_P/2+1]) >> 8<<8);
            }
            //*REG2_P = RAM_VAL;
            quint32 REG_SUM = RAM_VAL + *REG2_P;
            quint16 REG_SUM_FINAL = REG_SUM & 0xFFFF;
            if(REG_SUM_FINAL != REG_SUM){
                PS = PS | 0b00000000000001;
            }

            *REG2_P = REG_SUM_FINAL;
            break;
        }

        case (int) MOV_TYPES::
        REGISTER_TO_ADRESS:{
            break;
        }

        case (int) MOV_TYPES::
        NUMBER_TO_ADRESS:{
            break;
        }

        case (int) MOV_TYPES::
        ADRESS_TO_ADRESS:{

        }

        }


        break;
    }

    case SUB:{
        qDebug()<<"SUB";

        quint8 OP1 = (OP_CODE & 0xF000)>>12;
        quint8 OP2 = (OP_CODE & 0x0F00)>>8;

        quint8 TYPE = getOPType(OP1, OP2);
        switch (TYPE) {
        case (int) MOV_TYPES::REGISTER_TO_REGISTER:{
            quint16* REG1_P = &REG[OP1];
            quint16* REG2_P = &REG[OP2];
            quint32 REG_DIFF = -*REG1_P + *REG2_P;
            quint16 REG_DIFF_FINAL = REG_DIFF & 0xFFFF;
            if(REG_DIFF_FINAL != REG_DIFF){
                PS = PS | 0b00000000000001;
            }

            *REG2_P = REG_DIFF_FINAL;
            break;
        }

        case (int) MOV_TYPES::NUMBER_TO_REGISTER:{
            quint16* REG2_P = &REG[OP2];
            quint16 NUMBER = RAM[IR/2+1];
            quint32 REG_DIFF = -NUMBER + *REG2_P;
            quint16 REG_DIFF_FINAL = REG_DIFF & 0xFFFF;
            if(REG_DIFF_FINAL != REG_DIFF){
                PS = PS | 0b00000000000001;
            }

            *REG2_P = REG_DIFF_FINAL;
            PC+=2;
            //IR = PC;
            break;
        }

        }


        break;

    }

    case CMP: {
        qDebug()<<"CMP";

        quint8 OP1 = (OP_CODE & 0xF000)>>12;
        quint8 OP2 = (OP_CODE & 0x0F00)>>8;


        quint8 TYPE = getOPType(OP1, OP2);
        switch (TYPE) {
        case (int) MOV_TYPES::REGISTER_TO_REGISTER:{
            quint16* REG1_P = &REG[OP1];
            quint16* REG2_P = &REG[OP2];
            if(*REG1_P == *REG2_P){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & 0b0000000000000010;
            }
            break;
        }

        case (int) MOV_TYPES::NUMBER_TO_REGISTER:{
            quint16* REG2_P = &REG[OP2];
            quint16 NUMBER = RAM[IR/2+1];
            if(NUMBER == *REG2_P){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & 0b0000000000000010;
            }


            PC+=2;
            //IR = PC;
            break;
        }

        }


        break;

    }

    case MUL: {
        qDebug()<<"MUL";
        quint8 OP1 = (OP_CODE & 0xF000)>>12;
        quint8 OP2 = (OP_CODE & 0x0F00)>>8;

        quint8 TYPE = getOPType(OP1, OP2);
        switch (TYPE) {


        case (int)
        MOV_TYPES::REGISTER_TO_REGISTER:{
            quint16* REG1_P = &REG[OP1];
            quint16* REG2_P = &REG[OP2];
            quint64 REG_MUL = (*REG1_P) * (*REG2_P);
            quint16 REG_MUL_FINAL = REG_MUL & 0xFFFF;
            if(REG_MUL < UINT_MAX){
                if(REG_MUL_FINAL != REG_MUL){
                    PS = PS | 0b00000000000001;
                }

                *REG2_P = REG_MUL_FINAL;
            }
            else{
                *isRunning = false;
                qDebug()<<"Integer Overflow";
                emit evokeException("Integer Overflow");


            }

            break;
        }

        case (int) MOV_TYPES::NUMBER_TO_REGISTER:{
            quint16* REG2_P = &REG[OP2];
            quint16 NUMBER = RAM[IR/2+1];
            quint64 REG_MUL = NUMBER * (*REG2_P);
            quint16 REG_MUL_FINAL = REG_MUL & 0xFFFF;
            if(REG_MUL < UINT_MAX){
                if(REG_MUL_FINAL != REG_MUL){
                    PS = PS | 0b00000000000001;
                }

                *REG2_P = REG_MUL_FINAL;
            }
            else{
                *isRunning = false;
                qDebug()<<"Integer Overflow";
                emit evokeException("Integer Overflow");


            }

            break;

        }

        }

        break;
    }

    case DIV:{
        qDebug()<<"DIV";
        quint8 OP1 = (OP_CODE & 0xF000)>>12;
        quint8 OP2 = (OP_CODE & 0x0F00)>>8;
        quint8 TYPE = getOPType(OP1, OP2);
        switch (TYPE) {
        case (int) MOV_TYPES::REGISTER_TO_REGISTER:{
            quint16* REG1_P = &REG[OP1];
            quint16* REG2_P = &REG[OP2];
            quint32 REG_SUM = *REG2_P / *REG1_P;
            quint16 REG_SUM_FINAL = REG_SUM & 0xFFFF;
            if(REG_SUM_FINAL != REG_SUM){
                PS = PS | 0b00000000000001;
            }
            *REG2_P = REG_SUM_FINAL;
            break;
        }
        case (int) MOV_TYPES::NUMBER_TO_REGISTER:{
            quint16* REG2_P = &REG[OP2];
            quint16 NUMBER = RAM[IR/2+1];
            quint32 REG_SUM = *REG2_P / NUMBER;
            quint16 REG_SUM_FINAL = REG_SUM & 0xFFFF;
            if(REG_SUM_FINAL != REG_SUM){
                PS = PS | 0b00000000000001;
            }

            *REG2_P = REG_SUM_FINAL;
            PC+=2;
            //IR = PC;
            break;
        }
        }
        if( PC >= 64){
            *isRunning = false;
        }


        break;
    }
    }

    emit dataChanged();

}

void Processor::writeRAM(unsigned short *DATA){
    for(int i = 0; i < 128; i++){

        RAM[i] = DATA[i];
    }

    emit dataChanged();

}

void Processor::onStep(){
    step(&isRunning);
}

quint8 Processor::getOPType(quint8 OP1, quint8 OP2){
    //  qDebug()<<"OP1 = " << OP1 << " OP2 = "<<OP2<<"\n\n";
    if(OP1 >= 0 && OP1 <= 3 && OP2 >= 0 && OP2 <= 3){
        return  (quint8) MOV_TYPES::REGISTER_TO_REGISTER;

    }
    else if(OP1 >= 0 && OP1 <= 3 && OP2 >= 4 && OP2 <= 7 ){
        return (quint8) MOV_TYPES::REGISTER_TO_ADRESS;
    }

    else if(OP1 >= 4 && OP1 <=7 && OP2 >= 0 && OP2 <=3){
        return (quint8) MOV_TYPES::ADRESS_TO_REGISTER;
    }

    else if(OP1 >= 4 && OP1 <=7 &&  OP2 >= 4 && OP2 <= 7){
        return (quint8) MOV_TYPES::ADRESS_TO_ADRESS;
    }

    else if(OP1 == 0x000D && OP2 >= 0 && OP2 <= 3){
        return (quint8) MOV_TYPES::NUMBER_TO_REGISTER;
    }

    else if(OP1 == 0x000D && OP2 >= 4 && OP2 <= 7){
        return (quint8) MOV_TYPES::NUMBER_TO_ADRESS;
    }
    return (quint8) MOV_TYPES::UNDEFINED;
}

void Processor::reset(){
    REG[0] = 0x0000;
    REG[1] = 0x0000;
    REG[2] = 0x0000;
    REG[3] = 0x0000;
    PC = 0;
    IR = 0;
    for(int i = 0; i < 128; i++){
        RAM[i] = 0;
    }
    //memset(&RAM, 128, 2);
    emit dataChanged();
}

void Processor::resetCommand(){
    PC = 0;
    IR = 0;
    emit dataChanged();
}





