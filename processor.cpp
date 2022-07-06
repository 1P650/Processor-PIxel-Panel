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
    //unsigned short OP_CODE = RAM[IR/2];
    quint16 OP_CODE = RAM[IR/2];
    switch (OP_CODE & 0x00FF) {

    ///NOP
    case NOP:{

        qDebug()<<"NOP";
        break;
    }

    ///STOP
    case STOP:{

        qDebug()<<"STOP";
        PC-=2;
        *isRunning = false;
        break;
    }

    ///MOV
    case MOV:{
        qDebug()<<"MOV";

        quint8 OP1 = (OP_CODE & 0xF000)>>12;
        quint8 OP2 = (OP_CODE & 0x0F00)>>8;

        quint8 TYPE = getOPType(OP1, OP2);

        switch (TYPE) {

        case (int)
        MOV_TYPES::
        REGISTER_TO_REGISTER:{

            qDebug()<<"REGISTER TO REGISTER";

            quint16* REG1_P = &REG[OP1];
            quint16* REG2_P = &REG[OP2];
            *REG2_P = *REG1_P;

            if(*REG2_P == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }

            if ((*REG2_P) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            break;
        }

        case (int)
        MOV_TYPES::
        NUMBER_TO_REGISTER:{
            qDebug()<<"NUMBER TO REGISTER";
            quint16* REG2_P = &REG[OP2];
            *REG2_P = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]&0x00FF) << 8);

            if(*REG2_P == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }

            if ((*REG2_P) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            PC+=2;
            break;
        }

        case (int)
        MOV_TYPES::
        ADRESS_TO_REGISTER:{
            qDebug()<<"ADRESS TO REGISTER";

            quint16* REG1_P = &REG[OP1%4];
            quint16* REG2_P = &REG[OP2];
            if(*REG1_P % 2 == 0){
                quint16 RAM_VAL = RAM[*REG1_P/2]>>8 | ( (RAM[*REG1_P/2]&0x00FF) << 8);

                *REG2_P = RAM_VAL;
            }
            else{
                *REG2_P = (RAM[*REG1_P/2]&0x00FF>>8) | ( (RAM[*REG1_P/2+1]) >> 8<<8);
            }

            if(*REG2_P == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }

            if ((*REG2_P) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }


            break;
        }

        case (int)
        MOV_TYPES::
        REGISTER_TO_ADRESS:{
            qDebug()<<"REGISTER TO ADRESS";

            quint16* REG1_P = &REG[OP1];
            quint16* REG2_P = &REG[OP2%4];

            qDebug()<<*REG1_P<<" "<<*REG2_P;



            if(*REG2_P % 2 == 0){
                //RAM[*REG2_P/2] = *REG1_P<<8 | ( (*REG1_P>>8));
                RAM [*REG2_P/2] = *REG1_P;
            }
            else{
                qDebug()<<*REG2_P/2;
                RAM[*REG2_P/2] = (RAM[*REG2_P/2] & 0xFF00) | (*REG1_P & 0x00FF);

                RAM[*REG2_P/2 + 1] = (RAM[*REG2_P/2+1] & 0x00FF ) | (*REG1_P & 0xFF00) ;

            }

            if(*REG1_P == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }

            if ((*REG1_P) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }



            break;
        }

        case (int)
        MOV_TYPES::
        NUMBER_TO_ADRESS:
        {
            qDebug()<<"NUMBER TO ADRESS";

            quint16 NUMBER = RAM[IR/2+1];
            quint16* REG2_P = &REG[OP2%4];




            if(*REG2_P % 2 == 0){
                RAM[*REG2_P/2] = NUMBER;
            }
            else{

                RAM[*REG2_P/2] = (RAM[*REG2_P/2] & 0xFF00) | ((NUMBER & 0xFF00)>>8);

                RAM[*REG2_P/2 + 1] = (RAM[*REG2_P/2+1] & 0x00FF ) | ((NUMBER & 0x00FF)<<8);

            }

            if(NUMBER == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }

            if ((NUMBER) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            PC+=2;


            break;
        }




        case (int)
        MOV_TYPES::
        ADRESS_TO_ADRESS:{
            qDebug()<<"ADRESS TO ADRESS";

            quint16* REG1_P = &REG[OP1%4];
            quint16* REG2_P = &REG[OP2%4];
            quint16 RAM_VAL;

            if(*REG1_P % 2 == 0){
                RAM_VAL = RAM[*REG1_P/2];
            }
            else{
                RAM_VAL = ((RAM[*REG1_P/2]&0x00FF)<<8) | (RAM[*REG1_P/2 + 1]>>8);
            }


            if(*REG2_P % 2 == 0){
                RAM[*REG2_P/2] = RAM_VAL;
            }
            else{
                //qDebug()<<*REG2_P/2;
                RAM[*REG2_P/2] = (RAM_VAL >> 8);

                RAM[*REG2_P/2 + 1] = (RAM_VAL & 0x00FF)<<8;

            }

            if(RAM_VAL == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }

            if ((RAM_VAL) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }


            break;
        }



        }
        break;
    }

    ///ADD
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
            *REG2_P = REG_SUM_FINAL;

            //CF FLAG
            if(REG_SUM_FINAL != REG_SUM){
                PS = PS | 0b0000000000000100;
            }
            else{
                PS = PS & (~(0b0000000000000100));
            }
            //ZF FLAG
            if(REG_SUM_FINAL == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_SUM_FINAL) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }




            break;
        }

        case (int) MOV_TYPES::
        NUMBER_TO_REGISTER:{
            quint16* REG2_P = &REG[OP2];
            quint16 NUMBER = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]<<8));
            quint32 REG_SUM = NUMBER + *REG2_P;
            quint16 REG_SUM_FINAL = REG_SUM & 0xFFFF;


            *REG2_P = REG_SUM_FINAL;

            //CF FLAG
            if(REG_SUM_FINAL != REG_SUM){
                PS = PS | 0b0000000000000100;
            }
            else{
                PS = PS & (~(0b0000000000000100));
            }
            //ZF FLAG
            if(REG_SUM_FINAL == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_SUM_FINAL) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

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

                RAM_VAL = (RAM[*REG1_P/2]&0xFF00 >> 8) | ((RAM[(*REG1_P/2)+1]) >> 8<<8);

            }
            //*REG2_P = RAM_VAL;
            quint32 REG_SUM = RAM_VAL + *REG2_P;
            quint16 REG_SUM_FINAL = REG_SUM & 0xFFFF;


            *REG2_P = REG_SUM_FINAL;

            //CF FLAG
            if(REG_SUM_FINAL != REG_SUM){
                PS = PS | 0b0000000000000100;
            }
            else{
                PS = PS & (~(0b0000000000000100));
            }
            //ZF FLAG
            if(REG_SUM_FINAL == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_SUM_FINAL) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            break;
        }

        case (int) MOV_TYPES::

        REGISTER_TO_ADRESS:{
            qDebug()<<"REGISTER TO ADRESS";
            quint16* REG1_P = &REG[OP1];
            quint16* REG2_P = &REG[OP2%4];

            quint16 RAM_VAL;
            if(*REG2_P % 2 == 0){
                RAM_VAL = RAM[*REG2_P/2]>>8 | ( (RAM[*REG2_P/2]&0x00FF) << 8);   
            }
            else{
                RAM_VAL = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);
            }



            quint32 REG_SUM = RAM_VAL + *REG1_P;
            quint16 REG_SUM_FINAL = REG_SUM & 0xFFFF;




            if(*REG2_P % 2 == 0){
                RAM[*REG2_P/2] = REG_SUM_FINAL<<8 | ( (REG_SUM_FINAL>>8));
            }
            else{
                //qDebug()<<*REG2_P/2;
                RAM[*REG2_P/2] = (RAM[*REG2_P/2] & 0xFF00) | (REG_SUM_FINAL & 0x00FF);

                RAM[*REG2_P/2 + 1] = (RAM[*REG2_P/2+1] & 0x00FF ) | (REG_SUM_FINAL & 0xFF00) ;

            }

            //CF FLAG
            if(REG_SUM_FINAL != REG_SUM){
                PS = PS | 0b0000000000000100;
            }
            else{
                PS = PS & (~(0b0000000000000100));
            }
            //ZF FLAG
            if(REG_SUM_FINAL == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_SUM_FINAL) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }


            break;
        }

        case (int) MOV_TYPES::
        NUMBER_TO_ADRESS:{

            quint16 NUMBER = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]<<8));
            quint16* REG2_P = &REG[OP2%4];

            quint16 RAM_VAL;
            if(*REG2_P % 2 == 0){
                RAM_VAL = RAM[*REG2_P/2]>>8 | ( (RAM[*REG2_P/2]&0x00FF) << 8);
            }
            else{
                //RAM_VAL = (RAM[*REG2_P/2]<<8>>8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);
                RAM_VAL = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);

            }

            quint32 REG_SUM = RAM_VAL + NUMBER;
            quint16 REG_SUM_FINAL = REG_SUM & 0xFFFF;




            if(*REG2_P % 2 == 0){
                RAM[*REG2_P/2] = REG_SUM_FINAL<<8 | ( (REG_SUM_FINAL>>8));
            }
            else{
                //qDebug()<<*REG2_P/2;
                RAM[*REG2_P/2] = (RAM[*REG2_P/2] & 0xFF00) | (REG_SUM_FINAL & 0x00FF);

                RAM[*REG2_P/2 + 1] = (RAM[*REG2_P/2+1] & 0x00FF ) | (REG_SUM_FINAL & 0xFF00) ;

            }

            //CF FLAG
            if(REG_SUM_FINAL != REG_SUM){
                PS = PS | 0b0000000000000100;
            }
            else{
                PS = PS & (~(0b0000000000000100));
            }
            //ZF FLAG
            if(REG_SUM_FINAL == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_SUM_FINAL) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }


            PC+=2;
            break;
        }

        case (int) MOV_TYPES::
        ADRESS_TO_ADRESS:{

            quint16* REG1_P = &REG[OP1%4];
            quint16* REG2_P = &REG[OP2%4];


            quint16 RAM_VAL_1;
            if(*REG1_P % 2 == 0){
                RAM_VAL_1 = RAM[*REG1_P/2]>>8 | ( (RAM[*REG1_P/2]&0x00FF) << 8);
            }
            else{
                RAM_VAL_1 = (RAM[*REG1_P/2]&0xFF00 >> 8) | ((RAM[(*REG1_P/2)+1]) >> 8<<8);
            }

            quint16 RAM_VAL_2;
            if(*REG2_P % 2 == 0){
                RAM_VAL_2 = RAM[*REG2_P/2]>>8 | ( (RAM[*REG2_P/2]&0x00FF) << 8);
            }
            else{
                RAM_VAL_2 = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);
            }



            quint32 REG_SUM = RAM_VAL_2 + RAM_VAL_1;
            quint16 REG_SUM_FINAL = REG_SUM & 0xFFFF;




            if(*REG2_P % 2 == 0){
                RAM[*REG2_P/2] = REG_SUM_FINAL<<8 | ( (REG_SUM_FINAL>>8));
            }
            else{
                //qDebug()<<*REG2_P/2;
                RAM[*REG2_P/2] = (RAM[*REG2_P/2] & 0xFF00) | (REG_SUM_FINAL & 0x00FF);

                RAM[*REG2_P/2 + 1] = (RAM[*REG2_P/2+1] & 0x00FF ) | (REG_SUM_FINAL & 0xFF00) ;

            }


            //CF FLAG
            if(REG_SUM_FINAL != REG_SUM){
                PS = PS | 0b0000000000000100;
            }
            else{
                PS = PS & (~(0b0000000000000100));
            }
            //ZF FLAG
            if(REG_SUM_FINAL == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_SUM_FINAL) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }


            break;

        }

        }


        break;
    }
    ///SUB
    case SUB:{
       // qDebug()<<"SUB";

        quint8 OP1 = (OP_CODE & 0xF000)>>12;
        quint8 OP2 = (OP_CODE & 0x0F00)>>8;

        quint8 TYPE = getOPType(OP1, OP2);
        switch (TYPE) {

        case (int) MOV_TYPES::
        REGISTER_TO_REGISTER:{
            quint16* REG1_P = &REG[OP1];
            quint16* REG2_P = &REG[OP2];
            quint32 REG_DIFF = -*REG1_P + *REG2_P;
            quint16 REG_DIFF_FINAL = REG_DIFF & 0xFFFF;

            *REG2_P = REG_DIFF_FINAL;

            //CF FLAG
            if(REG_DIFF_FINAL != REG_DIFF){
                PS = PS | 0b0000000000000100;
            }
            else{
                PS = PS & (~(0b0000000000000100));
            }
            //ZF FLAG
            if(REG_DIFF_FINAL == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_DIFF_FINAL) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            break;
        }

        case (int) MOV_TYPES::
        NUMBER_TO_REGISTER:{
            quint16* REG2_P = &REG[OP2];
            quint16 NUMBER = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]<<8));
            quint32 REG_DIFF = -NUMBER + *REG2_P;
            quint16 REG_DIFF_FINAL = REG_DIFF & 0xFFFF;


            *REG2_P = REG_DIFF_FINAL;

            //CF FLAG
            if(REG_DIFF_FINAL != REG_DIFF){
                PS = PS | 0b0000000000000100;
            }
            else{
                PS = PS & (~(0b0000000000000100));
            }
            //ZF FLAG
            if(REG_DIFF_FINAL == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_DIFF_FINAL) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

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

                RAM_VAL = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);

            }
            //*REG2_P = RAM_VAL;
            quint32 REG_DIFF = -RAM_VAL + *REG2_P;
            quint16 REG_DIFF_FINAL = REG_DIFF & 0xFFFF;


            *REG2_P = REG_DIFF_FINAL;


            //CF FLAG
            if(REG_DIFF_FINAL != REG_DIFF){
                PS = PS | 0b0000000000000100;
            }
            else{
                PS = PS & (~(0b0000000000000100));
            }
            //ZF FLAG
            if(REG_DIFF_FINAL == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_DIFF_FINAL) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            break;
        }

        case (int) MOV_TYPES::

        REGISTER_TO_ADRESS:{
            qDebug()<<"REGISTER TO ADRESS";
            quint16* REG1_P = &REG[OP1];
            quint16* REG2_P = &REG[OP2%4];

            quint16 RAM_VAL;
            if(*REG2_P % 2 == 0){
                RAM_VAL = RAM[*REG2_P/2]>>8 | ( (RAM[*REG2_P/2]&0x00FF) << 8);
            }
            else{
                RAM_VAL = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);
            }



            quint32 REG_DIFF = -*REG1_P + RAM_VAL;
            quint16 REG_DIFF_FINAL = REG_DIFF & 0xFFFF;

            if(*REG2_P % 2 == 0){
                RAM[*REG2_P/2] = REG_DIFF_FINAL<<8 | ( (REG_DIFF_FINAL>>8));
            }
            else{
                //qDebug()<<*REG2_P/2;
                RAM[*REG2_P/2] = (RAM[*REG2_P/2] & 0xFF00) | (REG_DIFF_FINAL & 0x00FF);

                RAM[*REG2_P/2 + 1] = (RAM[*REG2_P/2+1] & 0x00FF ) | (REG_DIFF_FINAL & 0xFF00) ;

            }

            //CF FLAG
            if(REG_DIFF_FINAL != REG_DIFF){
                PS = PS | 0b0000000000000100;
            }
            else{
                PS = PS & (~(0b0000000000000100));
            }
            //ZF FLAG
            if(REG_DIFF_FINAL == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_DIFF_FINAL) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }


            break;
        }

        case (int) MOV_TYPES::
        NUMBER_TO_ADRESS:{

            quint16 NUMBER = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]<<8));
            quint16* REG2_P = &REG[OP2%4];

            quint16 RAM_VAL;
            if(*REG2_P % 2 == 0){
                RAM_VAL = RAM[*REG2_P/2]>>8 | ( (RAM[*REG2_P/2]&0x00FF) << 8);
            }
            else{
                //RAM_VAL = (RAM[*REG2_P/2]<<8>>8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);
                RAM_VAL = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);

            }

            quint32 REG_DIFF = -NUMBER + RAM_VAL;
            quint16 REG_DIFF_FINAL = REG_DIFF & 0xFFFF;

            if(*REG2_P % 2 == 0){
                RAM[*REG2_P/2] = REG_DIFF_FINAL<<8 | ( (REG_DIFF_FINAL>>8));
            }
            else{
                //qDebug()<<*REG2_P/2;
                RAM[*REG2_P/2] = (RAM[*REG2_P/2] & 0xFF00) | (REG_DIFF_FINAL & 0x00FF);

                RAM[*REG2_P/2 + 1] = (RAM[*REG2_P/2+1] & 0x00FF ) | (REG_DIFF_FINAL & 0xFF00) ;

            }

            //CF FLAG
            if(REG_DIFF_FINAL != REG_DIFF){
                PS = PS | 0b0000000000000100;
            }
            else{
                PS = PS & (~(0b0000000000000100));
            }
            //ZF FLAG
            if(REG_DIFF_FINAL == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_DIFF_FINAL) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            PC+=2;
            break;
        }

        case (int) MOV_TYPES::
        ADRESS_TO_ADRESS:{

            quint16* REG1_P = &REG[OP1%4];
            quint16* REG2_P = &REG[OP2%4];


            quint16 RAM_VAL_1;
            if(*REG1_P % 2 == 0){
                RAM_VAL_1 = RAM[*REG1_P/2]>>8 | ( (RAM[*REG1_P/2]&0x00FF) << 8);
            }
            else{
                RAM_VAL_1 = (RAM[*REG1_P/2]&0xFF00 >> 8) | ((RAM[(*REG1_P/2)+1]) >> 8<<8);
            }

            quint16 RAM_VAL_2;
            if(*REG2_P % 2 == 0){
                RAM_VAL_2 = RAM[*REG2_P/2]>>8 | ( (RAM[*REG2_P/2]&0x00FF) << 8);
            }
            else{
                RAM_VAL_2 = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);
            }



            quint32 REG_DIFF = -RAM_VAL_1 + RAM_VAL_2;
            quint16 REG_DIFF_FINAL = REG_DIFF & 0xFFFF;




            if(*REG2_P % 2 == 0){
                RAM[*REG2_P/2] = REG_DIFF_FINAL<<8 | ( (REG_DIFF_FINAL>>8));
            }
            else{
                //qDebug()<<*REG2_P/2;
                RAM[*REG2_P/2] = (RAM[*REG2_P/2] & 0xFF00) | (REG_DIFF_FINAL & 0x00FF);

                RAM[*REG2_P/2 + 1] = (RAM[*REG2_P/2+1] & 0x00FF ) | (REG_DIFF_FINAL & 0xFF00) ;

            }

            //CF FLAG
            if(REG_DIFF_FINAL != REG_DIFF){
                PS = PS | 0b0000000000000100;
            }
            else{
                PS = PS & (~(0b0000000000000100));
            }
            //ZF FLAG
            if(REG_DIFF_FINAL == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_DIFF_FINAL) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            break;
        }

        }


        break;

    }
    ///CMP
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
                PS = PS & ~(0b0000000000000010);
            }
            break;
        }

        case (int) MOV_TYPES::NUMBER_TO_REGISTER:{

            quint16* REG2_P = &REG[OP2];
            quint16 NUMBER = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]<<8));
            if(NUMBER == *REG2_P){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }


            PC+=2;
            //IR = PC;
            break;
        }

        case (int) MOV_TYPES::ADRESS_TO_REGISTER:{
            quint16* REG1_P = &REG[OP1%4];
            quint16* REG2_P = &REG[OP2];
            quint16 RAM_VAL;
            if(*REG1_P % 2 == 0){
                RAM_VAL = RAM[*REG1_P/2]>>8 | ( (RAM[*REG1_P/2]&0x00FF) << 8);
            }
            else{

                RAM_VAL = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);

            }

            if(RAM_VAL == *REG2_P){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            break;
        }

        case (int) MOV_TYPES::REGISTER_TO_ADRESS:{
            quint16* REG1_P = &REG[OP1];
            quint16* REG2_P = &REG[OP2%4];

            quint16 RAM_VAL;
            if(*REG2_P % 2 == 0){
                RAM_VAL = RAM[*REG2_P/2]>>8 | ( (RAM[*REG2_P/2]&0x00FF) << 8);
            }
            else{
                RAM_VAL = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);
            }
            if(RAM_VAL == *REG1_P){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            break;
        }

        case (int) MOV_TYPES::NUMBER_TO_ADRESS:{
            quint16 NUMBER = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]<<8));
            quint16* REG2_P = &REG[OP2%4];

            quint16 RAM_VAL;
            if(*REG2_P % 2 == 0){
                RAM_VAL = RAM[*REG2_P/2]>>8 | ( (RAM[*REG2_P/2]&0x00FF) << 8);
            }
            else{
                //RAM_VAL = (RAM[*REG2_P/2]<<8>>8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);
                RAM_VAL = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);

            }

            if(RAM_VAL == NUMBER){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }

            PC+=2;
            break;
        }

        case (int) MOV_TYPES::ADRESS_TO_ADRESS:{

            quint16* REG1_P = &REG[OP1%4];
            quint16* REG2_P = &REG[OP2%4];


            quint16 RAM_VAL_1;
            if(*REG1_P % 2 == 0){
                RAM_VAL_1 = RAM[*REG1_P/2]>>8 | ( (RAM[*REG1_P/2]&0x00FF) << 8);
            }
            else{
                RAM_VAL_1 = (RAM[*REG1_P/2]&0xFF00 >> 8) | ((RAM[(*REG1_P/2)+1]) >> 8<<8);
            }

            quint16 RAM_VAL_2;
            if(*REG2_P % 2 == 0){
                RAM_VAL_2 = RAM[*REG2_P/2]>>8 | ( (RAM[*REG2_P/2]&0x00FF) << 8);
            }
            else{
                RAM_VAL_2 = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);
            }

            if(RAM_VAL_1 == RAM_VAL_2){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }



            break;
        }

        }


        break;

    }
    ///MUL
    case MUL: {
        qDebug()<<"MUL";
        quint8 OP1 = (OP_CODE & 0xF000)>>12;
        quint8 OP2 = (OP_CODE & 0x0F00)>>8;

        quint8 TYPE = getOPType(OP1, OP2);
        switch (TYPE) {


        case (int)
        MOV_TYPES::
        REGISTER_TO_REGISTER:{
            quint16* REG1_P = &REG[OP1];
            quint16* REG2_P = &REG[OP2];
            quint64 REG_MUL = (*REG1_P) * (*REG2_P);
            quint16 REG_MUL_FINAL = REG_MUL & 0xFFFF;
            if(REG_MUL < UINT_MAX){


                *REG2_P = REG_MUL_FINAL;

                //CF FLAG
                if(REG_MUL_FINAL != REG_MUL){
                    PS = PS | 0b0000000000000100;
                }
                else{
                    PS = PS & (~(0b0000000000000100));
                }
                //ZF FLAG
                if(REG_MUL_FINAL == 0){
                    PS = PS | 0b0000000000000010;
                }
                else{
                    PS = PS & ~(0b0000000000000010);
                }
                //NF FLAG
                if ((REG_MUL_FINAL) >> 0xF == 1){
                    PS = PS | 0b0000000000000001;
                }
                else{
                    PS = PS & ~(0b0000000000000001);
                }


            }
            else{
                *isRunning = false;
                qDebug()<<"Integer Overflow";
                emit evokeException("Integer Overflow");


            }

            break;
        }

        case (int)
        MOV_TYPES::
        NUMBER_TO_REGISTER:{
            quint16* REG2_P = &REG[OP2];
            //quint16 NUMBER = RAM[IR/2+1];
            quint16 NUMBER = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]<<8));


            quint64 REG_MUL = NUMBER * (*REG2_P);
            quint16 REG_MUL_FINAL = REG_MUL & 0xFFFF;
            if(REG_MUL < UINT_MAX){

                *REG2_P = REG_MUL_FINAL;

                //CF FLAG
                if(REG_MUL_FINAL != REG_MUL){
                    PS = PS | 0b0000000000000100;
                }
                else{
                    PS = PS & (~(0b0000000000000100));
                }
                //ZF FLAG
                if(REG_MUL_FINAL == 0){
                    PS = PS | 0b0000000000000010;
                }
                else{
                    PS = PS & ~(0b0000000000000010);
                }
                //NF FLAG
                if ((REG_MUL_FINAL) >> 0xF == 1){
                    PS = PS | 0b0000000000000001;
                }
                else{
                    PS = PS & ~(0b0000000000000001);
                }
            }
            else{
                *isRunning = false;
                qDebug()<<"Integer Overflow";
                emit evokeException("Integer Overflow");


            }

            break;

        }

        case (int)
        MOV_TYPES::
        ADRESS_TO_REGISTER:{
        quint16* REG1_P = &REG[OP1%4];
        quint16* REG2_P = &REG[OP2];
        quint16 RAM_VAL;
        if(*REG1_P % 2 == 0){
            RAM_VAL = RAM[*REG1_P/2]>>8 | ( (RAM[*REG1_P/2]&0x00FF) << 8);
        }
        else{

            RAM_VAL = (RAM[*REG1_P/2]&0xFF00 >> 8) | ((RAM[(*REG1_P/2)+1]) >> 8<<8);

        }

            quint64 REG_MUL = RAM_VAL * (*REG2_P);
            quint16 REG_MUL_FINAL = REG_MUL & 0xFFFF;
            if(REG_MUL < UINT_MAX){


                *REG2_P = REG_MUL_FINAL;

                //CF FLAG
                if(REG_MUL_FINAL != REG_MUL){
                    PS = PS | 0b0000000000000100;
                }
                else{
                    PS = PS & (~(0b0000000000000100));
                }
                //ZF FLAG
                if(REG_MUL_FINAL == 0){
                    PS = PS | 0b0000000000000010;
                }
                else{
                    PS = PS & ~(0b0000000000000010);
                }
                //NF FLAG
                if ((REG_MUL_FINAL) >> 0xF == 1){
                    PS = PS | 0b0000000000000001;
                }
                else{
                    PS = PS & ~(0b0000000000000001);
                }
            }
            else{
                *isRunning = false;
                qDebug()<<"Integer Overflow";
                emit evokeException("Integer Overflow");


            }

            break;

        }


        case (int)
        MOV_TYPES::
        REGISTER_TO_ADRESS:{
            quint16* REG1_P = &REG[OP1];
            quint16* REG2_P = &REG[OP2%4];

            quint16 RAM_VAL;
            if(*REG2_P % 2 == 0){
                RAM_VAL = RAM[*REG2_P/2]>>8 | ( (RAM[*REG2_P/2]&0x00FF) << 8);
            }
            else{
                RAM_VAL = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);
            }

            quint64 REG_MUL = RAM_VAL * (*REG1_P);
            quint16 REG_MUL_FINAL = REG_MUL & 0xFFFF;
            if(REG_MUL < UINT_MAX){


                if(*REG2_P % 2 == 0){
                    RAM[*REG2_P/2] = REG_MUL_FINAL<<8 | ( (REG_MUL_FINAL>>8));
                }
                else{
                    //qDebug()<<*REG2_P/2;
                    RAM[*REG2_P/2] = (RAM[*REG2_P/2] & 0xFF00) | (REG_MUL_FINAL & 0x00FF);

                    RAM[*REG2_P/2 + 1] = (RAM[*REG2_P/2+1] & 0x00FF ) | (REG_MUL_FINAL & 0xFF00) ;

                }


                //CF FLAG
                if(REG_MUL_FINAL != REG_MUL){
                    PS = PS | 0b0000000000000100;
                }
                else{
                    PS = PS & (~(0b0000000000000100));
                }
                //ZF FLAG
                if(REG_MUL_FINAL == 0){
                    PS = PS | 0b0000000000000010;
                }
                else{
                    PS = PS & ~(0b0000000000000010);
                }
                //NF FLAG
                if ((REG_MUL_FINAL) >> 0xF == 1){
                    PS = PS | 0b0000000000000001;
                }
                else{
                    PS = PS & ~(0b0000000000000001);
                }
            }
            else{
                *isRunning = false;
                qDebug()<<"Integer Overflow";
                emit evokeException("Integer Overflow");


            }

            break;

        }

        case (int)
        MOV_TYPES::
        NUMBER_TO_ADRESS:{
            quint16 NUMBER = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]<<8));
            quint16* REG2_P = &REG[OP2%4];

            quint16 RAM_VAL;
            if(*REG2_P % 2 == 0){
                RAM_VAL = RAM[*REG2_P/2]>>8 | ( (RAM[*REG2_P/2]&0x00FF) << 8);
            }
            else{
                //RAM_VAL = (RAM[*REG2_P/2]<<8>>8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);
                RAM_VAL = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);

            }

            quint64 REG_MUL = NUMBER * (RAM_VAL);
            quint16 REG_MUL_FINAL = REG_MUL & 0xFFFF;
            if(REG_MUL < UINT_MAX){


                if(*REG2_P % 2 == 0){
                    RAM[*REG2_P/2] = REG_MUL_FINAL<<8 | ( (REG_MUL_FINAL>>8));
                }
                else{
                    //qDebug()<<*REG2_P/2;
                    RAM[*REG2_P/2] = (RAM[*REG2_P/2] & 0xFF00) | (REG_MUL_FINAL & 0x00FF);

                    RAM[*REG2_P/2 + 1] = (RAM[*REG2_P/2+1] & 0x00FF ) | (REG_MUL_FINAL & 0xFF00) ;

                }

                //CF FLAG
                if(REG_MUL_FINAL != REG_MUL){
                    PS = PS | 0b0000000000000100;
                }
                else{
                    PS = PS & (~(0b0000000000000100));
                }
                //ZF FLAG
                if(REG_MUL_FINAL == 0){
                    PS = PS | 0b0000000000000010;
                }
                else{
                    PS = PS & ~(0b0000000000000010);
                }
                //NF FLAG
                if ((REG_MUL_FINAL) >> 0xF == 1){
                    PS = PS | 0b0000000000000001;
                }
                else{
                    PS = PS & ~(0b0000000000000001);
                }

                PC+=2;

            }
            else{
                *isRunning = false;
                qDebug()<<"Integer Overflow";
                emit evokeException("Integer Overflow");


            }

            break;

        }

        case (int)
        MOV_TYPES::
        ADRESS_TO_ADRESS:{
            quint16* REG1_P = &REG[OP1%4];
            quint16* REG2_P = &REG[OP2%4];


            quint16 RAM_VAL_1;
            if(*REG1_P % 2 == 0){
                RAM_VAL_1 = RAM[*REG1_P/2]>>8 | ( (RAM[*REG1_P/2]&0x00FF) << 8);
            }
            else{
                RAM_VAL_1 = (RAM[*REG1_P/2]&0xFF00 >> 8) | ((RAM[(*REG1_P/2)+1]) >> 8<<8);
            }

            quint16 RAM_VAL_2;
            if(*REG2_P % 2 == 0){
                RAM_VAL_2 = RAM[*REG2_P/2]>>8 | ( (RAM[*REG2_P/2]&0x00FF) << 8);
            }
            else{
                RAM_VAL_2 = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);
            }

            quint64 REG_MUL = RAM_VAL_1 * RAM_VAL_2;
            quint16 REG_MUL_FINAL = REG_MUL & 0xFFFF;
            if(REG_MUL < UINT_MAX){


                if(*REG2_P % 2 == 0){
                    RAM[*REG2_P/2] = REG_MUL_FINAL<<8 | ( (REG_MUL_FINAL>>8));
                }
                else{
                    //qDebug()<<*REG2_P/2;
                    RAM[*REG2_P/2] = (RAM[*REG2_P/2] & 0xFF00) | (REG_MUL_FINAL & 0x00FF);

                    RAM[*REG2_P/2 + 1] = (RAM[*REG2_P/2+1] & 0x00FF ) | (REG_MUL_FINAL & 0xFF00) ;

                }


                //CF FLAG
                if(REG_MUL_FINAL != REG_MUL){
                    PS = PS | 0b0000000000000100;
                }
                else{
                    PS = PS & (~(0b0000000000000100));
                }
                //ZF FLAG
                if(REG_MUL_FINAL == 0){
                    PS = PS | 0b0000000000000010;
                }
                else{
                    PS = PS & ~(0b0000000000000010);
                }
                //NF FLAG
                if ((REG_MUL_FINAL) >> 0xF == 1){
                    PS = PS | 0b0000000000000001;
                }
                else{
                    PS = PS & ~(0b0000000000000001);
                }
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
    ///DIV
    case DIV:{
        qDebug()<<"DIV";
        quint8 OP1 = (OP_CODE & 0xF000)>>12;
        quint8 OP2 = (OP_CODE & 0x0F00)>>8;
        quint8 TYPE = getOPType(OP1, OP2);

        switch (TYPE) {
        case (int)
        MOV_TYPES::
        REGISTER_TO_REGISTER:{
            quint16* REG1_P = &REG[OP1];
            quint16* REG2_P = &REG[OP2];
            quint32 REG_DIV = *REG2_P / *REG1_P;
            quint16 REG_DIV_FINAL = REG_DIV & 0xFFFF;

            *REG2_P = REG_DIV_FINAL;

            //CF FLAG
            if(REG_DIV_FINAL != REG_DIV){
                PS = PS | 0b0000000000000100;
            }
            else{
                PS = PS & (~(0b0000000000000100));
            }
            //ZF FLAG
            if(REG_DIV_FINAL == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_DIV_FINAL) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            break;
        }
        case (int)
        MOV_TYPES::
        NUMBER_TO_REGISTER:{
            quint16* REG2_P = &REG[OP2];
            quint16 NUMBER = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]<<8));
            quint32 REG_DIV = *REG2_P / NUMBER;
            quint16 REG_DIV_FINAL = REG_DIV & 0xFFFF;


            *REG2_P = REG_DIV_FINAL;

            //CF FLAG
            if(REG_DIV_FINAL != REG_DIV){
                PS = PS | 0b0000000000000100;
            }
            else{
                PS = PS & (~(0b0000000000000100));
            }
            //ZF FLAG
            if(REG_DIV_FINAL == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_DIV_FINAL) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            PC+=2;
            //IR = PC;
            break;
        }

        case (int)
        MOV_TYPES::
        ADRESS_TO_REGISTER:{
            quint16* REG1_P = &REG[OP1%4];
            quint16* REG2_P = &REG[OP2];
            quint16 RAM_VAL;
            if(*REG1_P % 2 == 0){
                RAM_VAL = RAM[*REG1_P/2]>>8 | ( (RAM[*REG1_P/2]&0x00FF) << 8);
            }
            else{

                RAM_VAL = (RAM[*REG1_P/2]&0xFF00 >> 8) | ((RAM[(*REG1_P/2)+1]) >> 8<<8);

            }
            quint32 REG_DIV = *REG2_P / RAM_VAL;
            quint16 REG_DIV_FINAL = REG_DIV & 0xFFFF;



            *REG2_P = REG_DIV_FINAL;

            //CF FLAG
            if(REG_DIV_FINAL != REG_DIV){
                PS = PS | 0b0000000000000100;
            }
            else{
                PS = PS & (~(0b0000000000000100));
            }
            //ZF FLAG
            if(REG_DIV_FINAL == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_DIV_FINAL) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            //IR = PC;
            break;
        }

        case (int)
        MOV_TYPES::
        REGISTER_TO_ADRESS:{
            quint16* REG1_P = &REG[OP1];
            quint16* REG2_P = &REG[OP2%4];

            quint16 RAM_VAL;
            if(*REG2_P % 2 == 0){
                RAM_VAL = RAM[*REG2_P/2]>>8 | ( (RAM[*REG2_P/2]&0x00FF) << 8);
            }
            else{
                RAM_VAL = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);
            }


            quint32 REG_DIV = RAM_VAL / *REG1_P;
            quint16 REG_DIV_FINAL = REG_DIV & 0xFFFF;


            if(*REG2_P % 2 == 0){
                RAM[*REG2_P/2] = REG_DIV_FINAL<<8 | ( (REG_DIV_FINAL>>8));
            }
            else{
                //qDebug()<<*REG2_P/2;
                RAM[*REG2_P/2] = (RAM[*REG2_P/2] & 0xFF00) | (REG_DIV_FINAL & 0x00FF);

                RAM[*REG2_P/2 + 1] = (RAM[*REG2_P/2+1] & 0x00FF ) | (REG_DIV_FINAL & 0xFF00) ;

            }

            //CF FLAG
            if(REG_DIV_FINAL != REG_DIV){
                PS = PS | 0b0000000000000100;
            }
            else{
                PS = PS & (~(0b0000000000000100));
            }
            //ZF FLAG
            if(REG_DIV_FINAL == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_DIV_FINAL) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            //IR = PC;
            break;
        }


        case (int)
        MOV_TYPES::
        NUMBER_TO_ADRESS:{
            quint16 NUMBER = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]<<8));
            quint16* REG2_P = &REG[OP2%4];

            quint16 RAM_VAL;
            if(*REG2_P % 2 == 0){
                RAM_VAL = RAM[*REG2_P/2]>>8 | ( (RAM[*REG2_P/2]&0x00FF) << 8);
            }
            else{
                //RAM_VAL = (RAM[*REG2_P/2]<<8>>8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);
                RAM_VAL = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);

            }

            quint32 REG_DIV = RAM_VAL / NUMBER;
            quint16 REG_DIV_FINAL = REG_DIV & 0xFFFF;


            if(*REG2_P % 2 == 0){
                RAM[*REG2_P/2] = REG_DIV_FINAL<<8 | ( (REG_DIV_FINAL>>8));
            }
            else{
                //qDebug()<<*REG2_P/2;
                RAM[*REG2_P/2] = (RAM[*REG2_P/2] & 0xFF00) | (REG_DIV_FINAL & 0x00FF);

                RAM[*REG2_P/2 + 1] = (RAM[*REG2_P/2+1] & 0x00FF ) | (REG_DIV_FINAL & 0xFF00) ;

            }

            //CF FLAG
            if(REG_DIV_FINAL != REG_DIV){
                PS = PS | 0b0000000000000100;
            }
            else{
                PS = PS & (~(0b0000000000000100));
            }
            //ZF FLAG
            if(REG_DIV_FINAL == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_DIV_FINAL) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            PC+=2;
            //IR = PC;
            break;
        }

        case (int) MOV_TYPES::ADRESS_TO_ADRESS:{
            quint16* REG1_P = &REG[OP1%4];
            quint16* REG2_P = &REG[OP2%4];


            quint16 RAM_VAL_1;
            if(*REG1_P % 2 == 0){
                RAM_VAL_1 = RAM[*REG1_P/2]>>8 | ( (RAM[*REG1_P/2]&0x00FF) << 8);
            }
            else{
                RAM_VAL_1 = (RAM[*REG1_P/2]&0xFF00 >> 8) | ((RAM[(*REG1_P/2)+1]) >> 8<<8);
            }

            quint16 RAM_VAL_2;
            if(*REG2_P % 2 == 0){
                RAM_VAL_2 = RAM[*REG2_P/2]>>8 | ( (RAM[*REG2_P/2]&0x00FF) << 8);
            }
            else{
                RAM_VAL_2 = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);
            }



            quint32 REG_DIV = RAM_VAL_2/RAM_VAL_1;
            quint16 REG_DIV_FINAL = REG_DIV & 0xFFFF;


            if(*REG2_P % 2 == 0){
                RAM[*REG2_P/2] = REG_DIV_FINAL<<8 | ( (REG_DIV_FINAL>>8));
            }
            else{
                //qDebug()<<*REG2_P/2;
                RAM[*REG2_P/2] = (RAM[*REG2_P/2] & 0xFF00) | (REG_DIV_FINAL & 0x00FF);

                RAM[*REG2_P/2 + 1] = (RAM[*REG2_P/2+1] & 0x00FF ) | (REG_DIV_FINAL & 0xFF00) ;

            }

            //CF FLAG
            if(REG_DIV_FINAL != REG_DIV){
                PS = PS | 0b0000000000000100;
            }
            else{
                PS = PS & (~(0b0000000000000100));
            }
            //ZF FLAG
            if(REG_DIV_FINAL == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_DIV_FINAL) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            //IR = PC;
            break;
        }

        }

        break;
    }
    ///AND
    case AND:{
        quint8 OP1 = (OP_CODE & 0xF000)>>12;
        quint8 OP2 = (OP_CODE & 0x0F00)>>8;

        quint8 TYPE = getOPType(OP1, OP2);
        switch (TYPE) {

        case (int) MOV_TYPES::REGISTER_TO_REGISTER:{
            quint16* REG1_P = &REG[OP1];
            quint16* REG2_P = &REG[OP2];

            quint16 REG_AND =  *REG1_P & *REG2_P;
            *REG2_P = REG_AND;


            //ZF FLAG
            if(REG_AND == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_AND) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            break;
        }

        case (int) MOV_TYPES::NUMBER_TO_REGISTER:{
            quint16* REG2_P = &REG[OP2];
            quint16 NUMBER = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]<<8));

            quint16 REG_AND = *REG2_P & NUMBER;
            *REG2_P = REG_AND;

            //ZF FLAG
            if(REG_AND == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_AND) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            PC+=2;
            break;
        }
        case (int) MOV_TYPES::ADRESS_TO_REGISTER:{
            quint16* REG1_P = &REG[OP1%4];
            quint16* REG2_P = &REG[OP2];
            quint16 RAM_VAL;
            if(*REG1_P % 2 == 0){
                RAM_VAL = RAM[*REG1_P/2]>>8 | ( (RAM[*REG1_P/2]&0x00FF) << 8);
            }
            else{

                RAM_VAL = (RAM[*REG1_P/2]&0xFF00 >> 8) | ((RAM[(*REG1_P/2)+1]) >> 8<<8);

            }

            quint16 REG_AND = RAM_VAL & *REG2_P;
            *REG2_P = REG_AND;

            //ZF FLAG
            if(REG_AND == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_AND) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            break;
        }
        case (int) MOV_TYPES::REGISTER_TO_ADRESS:{
            quint16* REG1_P = &REG[OP1];
            quint16* REG2_P = &REG[OP2%4];

            quint16 RAM_VAL;
            if(*REG2_P % 2 == 0){
                RAM_VAL = RAM[*REG2_P/2]>>8 | ( (RAM[*REG2_P/2]&0x00FF) << 8);
            }
            else{
                RAM_VAL = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);
            }

            quint16 REG_AND = *REG1_P & RAM_VAL;



            if(*REG2_P % 2 == 0){
                RAM[*REG2_P/2] = REG_AND<<8 | ( (REG_AND>>8));
            }
            else{
                //qDebug()<<*REG2_P/2;
                RAM[*REG2_P/2] = (RAM[*REG2_P/2] & 0xFF00) | (REG_AND & 0x00FF);

                RAM[*REG2_P/2 + 1] = (RAM[*REG2_P/2+1] & 0x00FF ) | (REG_AND & 0xFF00) ;

            }

            //ZF FLAG
            if(REG_AND == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_AND) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }
            break;
        }
        case (int) MOV_TYPES::NUMBER_TO_ADRESS:{
            quint16 NUMBER = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]<<8));
            quint16* REG2_P = &REG[OP2%4];

            quint16 RAM_VAL;
            if(*REG2_P % 2 == 0){
                RAM_VAL = RAM[*REG2_P/2]>>8 | ( (RAM[*REG2_P/2]&0x00FF) << 8);
            }
            else{
                //RAM_VAL = (RAM[*REG2_P/2]<<8>>8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);
                RAM_VAL = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);

            }

            quint16 REG_AND = RAM_VAL & NUMBER;


            if(*REG2_P % 2 == 0){
                RAM[*REG2_P/2] = REG_AND<<8 | ( (REG_AND>>8));
            }
            else{
                //qDebug()<<*REG2_P/2;
                RAM[*REG2_P/2] = (RAM[*REG2_P/2] & 0xFF00) | (REG_AND & 0x00FF);

                RAM[*REG2_P/2 + 1] = (RAM[*REG2_P/2+1] & 0x00FF ) | (REG_AND & 0xFF00) ;

            }

            //ZF FLAG
            if(REG_AND == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_AND) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            PC+=2;
            break;
        }
        case (int) MOV_TYPES::ADRESS_TO_ADRESS:{
            quint16* REG1_P = &REG[OP1%4];
            quint16* REG2_P = &REG[OP2%4];


            quint16 RAM_VAL_1;
            if(*REG1_P % 2 == 0){
                RAM_VAL_1 = RAM[*REG1_P/2]>>8 | ( (RAM[*REG1_P/2]&0x00FF) << 8);
            }
            else{
                RAM_VAL_1 = (RAM[*REG1_P/2]&0xFF00 >> 8) | ((RAM[(*REG1_P/2)+1]) >> 8<<8);
            }

            quint16 RAM_VAL_2;
            if(*REG2_P % 2 == 0){
                RAM_VAL_2 = RAM[*REG2_P/2]>>8 | ( (RAM[*REG2_P/2]&0x00FF) << 8);
            }
            else{
                RAM_VAL_2 = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);
            }

            quint16 REG_AND = RAM_VAL_1 & RAM_VAL_2;

            if(*REG2_P % 2 == 0){
                RAM[*REG2_P/2] = REG_AND<<8 | ( (REG_AND>>8));
            }
            else{
                //qDebug()<<*REG2_P/2;
                RAM[*REG2_P/2] = (RAM[*REG2_P/2] & 0xFF00) | (REG_AND & 0x00FF);

                RAM[*REG2_P/2 + 1] = (RAM[*REG2_P/2+1] & 0x00FF ) | (REG_AND & 0xFF00) ;

            }

            //ZF FLAG
            if(REG_AND == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_AND) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }


            break;
        }



        }

        break;

    }
    ///OR
    case OR:{
        quint8 OP1 = (OP_CODE & 0xF000)>>12;
        quint8 OP2 = (OP_CODE & 0x0F00)>>8;

        quint8 TYPE = getOPType(OP1, OP2);
        switch (TYPE) {

        case (int) MOV_TYPES::REGISTER_TO_REGISTER:{
            quint16* REG1_P = &REG[OP1];
            quint16* REG2_P = &REG[OP2];

            quint16 REG_OR =  *REG1_P | *REG2_P;
            *REG2_P = REG_OR;

            //ZF FLAG
            if(REG_OR == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_OR) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            break;
        }

        case (int) MOV_TYPES::NUMBER_TO_REGISTER:{
            quint16* REG2_P = &REG[OP2];
            quint16 NUMBER = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]<<8));

            quint16 REG_OR = *REG2_P | NUMBER;
            *REG2_P = REG_OR;

            //ZF FLAG
            if(REG_OR == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_OR) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }


            PC+=2;
            break;
        }
        case (int) MOV_TYPES::ADRESS_TO_REGISTER:{
            quint16* REG1_P = &REG[OP1%4];
            quint16* REG2_P = &REG[OP2];
            quint16 RAM_VAL;
            if(*REG1_P % 2 == 0){
                RAM_VAL = RAM[*REG1_P/2]>>8 | ( (RAM[*REG1_P/2]&0x00FF) << 8);
            }
            else{

                RAM_VAL = (RAM[*REG1_P/2]&0xFF00 >> 8) | ((RAM[(*REG1_P/2)+1]) >> 8<<8);

            }

            quint16 REG_OR = RAM_VAL | *REG2_P;
            *REG2_P = REG_OR;

            //ZF FLAG
            if(REG_OR == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_OR) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }


            break;
        }
        case (int) MOV_TYPES::REGISTER_TO_ADRESS:{
            quint16* REG1_P = &REG[OP1];
            quint16* REG2_P = &REG[OP2%4];

            quint16 RAM_VAL;
            if(*REG2_P % 2 == 0){
                RAM_VAL = RAM[*REG2_P/2]>>8 | ( (RAM[*REG2_P/2]&0x00FF) << 8);
            }
            else{
                RAM_VAL = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);
            }

            quint16 REG_OR = *REG1_P | RAM_VAL;



            if(*REG2_P % 2 == 0){
                RAM[*REG2_P/2] = REG_OR<<8 | ( (REG_OR>>8));
            }
            else{
                //qDebug()<<*REG2_P/2;
                RAM[*REG2_P/2] = (RAM[*REG2_P/2] & 0xFF00) | (REG_OR & 0x00FF);

                RAM[*REG2_P/2 + 1] = (RAM[*REG2_P/2+1] & 0x00FF ) | (REG_OR & 0xFF00) ;

            }

            //ZF FLAG
            if(REG_OR == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_OR) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            break;
        }
        case (int) MOV_TYPES::NUMBER_TO_ADRESS:{
            quint16 NUMBER = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]<<8));
            quint16* REG2_P = &REG[OP2%4];

            quint16 RAM_VAL;
            if(*REG2_P % 2 == 0){
                RAM_VAL = RAM[*REG2_P/2]>>8 | ( (RAM[*REG2_P/2]&0x00FF) << 8);
            }
            else{
                //RAM_VAL = (RAM[*REG2_P/2]<<8>>8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);
                RAM_VAL = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);

            }

            quint16 REG_OR = RAM_VAL | NUMBER;


            if(*REG2_P % 2 == 0){
                RAM[*REG2_P/2] = REG_OR<<8 | ( (REG_OR>>8));
            }
            else{
                //qDebug()<<*REG2_P/2;
                RAM[*REG2_P/2] = (RAM[*REG2_P/2] & 0xFF00) | (REG_OR & 0x00FF);

                RAM[*REG2_P/2 + 1] = (RAM[*REG2_P/2+1] & 0x00FF ) | (REG_OR & 0xFF00) ;

            }

            //ZF FLAG
            if(REG_OR == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_OR) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }


            PC+=2;
            break;
        }
        case (int) MOV_TYPES::ADRESS_TO_ADRESS:{
            quint16* REG1_P = &REG[OP1%4];
            quint16* REG2_P = &REG[OP2%4];


            quint16 RAM_VAL_1;
            if(*REG1_P % 2 == 0){
                RAM_VAL_1 = RAM[*REG1_P/2]>>8 | ( (RAM[*REG1_P/2]&0x00FF) << 8);
            }
            else{
                RAM_VAL_1 = (RAM[*REG1_P/2]&0xFF00 >> 8) | ((RAM[(*REG1_P/2)+1]) >> 8<<8);
            }

            quint16 RAM_VAL_2;
            if(*REG2_P % 2 == 0){
                RAM_VAL_2 = RAM[*REG2_P/2]>>8 | ( (RAM[*REG2_P/2]&0x00FF) << 8);
            }
            else{
                RAM_VAL_2 = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);
            }

            quint16 REG_OR = RAM_VAL_1 | RAM_VAL_2;

            if(*REG2_P % 2 == 0){
                RAM[*REG2_P/2] = REG_OR<<8 | ( (REG_OR>>8));
            }
            else{
                //qDebug()<<*REG2_P/2;
                RAM[*REG2_P/2] = (RAM[*REG2_P/2] & 0xFF00) | (REG_OR & 0x00FF);

                RAM[*REG2_P/2 + 1] = (RAM[*REG2_P/2+1] & 0x00FF ) | (REG_OR & 0xFF00) ;

            }


            //ZF FLAG
            if(REG_OR == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_OR) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            break;
        }



        }



        break;
    }
    ///XOR
    case XOR:{
        quint8 OP1 = (OP_CODE & 0xF000)>>12;
        quint8 OP2 = (OP_CODE & 0x0F00)>>8;

        quint8 TYPE = getOPType(OP1, OP2);
        switch (TYPE) {

        case (int) MOV_TYPES::REGISTER_TO_REGISTER:{
            quint16* REG1_P = &REG[OP1];
            quint16* REG2_P = &REG[OP2];

            quint16 REG_XOR =  *REG1_P ^ *REG2_P;
            *REG2_P = REG_XOR;

            //ZF FLAG
            if(REG_XOR == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_XOR) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            break;
        }

        case (int) MOV_TYPES::NUMBER_TO_REGISTER:{
            quint16* REG2_P = &REG[OP2];
            quint16 NUMBER = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]<<8));

            quint16 REG_XOR = *REG2_P ^ NUMBER;
            *REG2_P = REG_XOR;

            //ZF FLAG
            if(REG_XOR == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_XOR) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }


            PC+=2;
            break;
        }
        case (int) MOV_TYPES::ADRESS_TO_REGISTER:{
            quint16* REG1_P = &REG[OP1%4];
            quint16* REG2_P = &REG[OP2];
            quint16 RAM_VAL;
            if(*REG1_P % 2 == 0){
                RAM_VAL = RAM[*REG1_P/2]>>8 | ( (RAM[*REG1_P/2]&0x00FF) << 8);
            }
            else{

                RAM_VAL = (RAM[*REG1_P/2]&0xFF00 >> 8) | ((RAM[(*REG1_P/2)+1]) >> 8<<8);

            }

            quint16 REG_XOR = RAM_VAL ^ *REG2_P;
            *REG2_P = REG_XOR;

            //ZF FLAG
            if(REG_XOR == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_XOR) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }


            break;
        }
        case (int) MOV_TYPES::REGISTER_TO_ADRESS:{
            quint16* REG1_P = &REG[OP1];
            quint16* REG2_P = &REG[OP2%4];

            quint16 RAM_VAL;
            if(*REG2_P % 2 == 0){
                RAM_VAL = RAM[*REG2_P/2]>>8 | ( (RAM[*REG2_P/2]&0x00FF) << 8);
            }
            else{
                RAM_VAL = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);
            }

            quint16 REG_XOR = *REG1_P ^ RAM_VAL;



            if(*REG2_P % 2 == 0){
                RAM[*REG2_P/2] = REG_XOR<<8 | ( (REG_XOR>>8));
            }
            else{
                //qDebug()<<*REG2_P/2;
                RAM[*REG2_P/2] = (RAM[*REG2_P/2] & 0xFF00) | (REG_XOR & 0x00FF);

                RAM[*REG2_P/2 + 1] = (RAM[*REG2_P/2+1] & 0x00FF ) | (REG_XOR & 0xFF00) ;

            }

            //ZF FLAG
            if(REG_XOR == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_XOR) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }


            break;
        }
        case (int) MOV_TYPES::NUMBER_TO_ADRESS:{
            quint16 NUMBER = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]<<8));
            quint16* REG2_P = &REG[OP2%4];

            quint16 RAM_VAL;
            if(*REG2_P % 2 == 0){
                RAM_VAL = RAM[*REG2_P/2]>>8 | ( (RAM[*REG2_P/2]&0x00FF) << 8);
            }
            else{
                //RAM_VAL = (RAM[*REG2_P/2]<<8>>8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);
                RAM_VAL = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);

            }

            quint16 REG_XOR = RAM_VAL ^ NUMBER;


            if(*REG2_P % 2 == 0){
                RAM[*REG2_P/2] = REG_XOR<<8 | ( (REG_XOR>>8));
            }
            else{
                //qDebug()<<*REG2_P/2;
                RAM[*REG2_P/2] = (RAM[*REG2_P/2] & 0xFF00) | (REG_XOR & 0x00FF);

                RAM[*REG2_P/2 + 1] = (RAM[*REG2_P/2+1] & 0x00FF ) | (REG_XOR & 0xFF00) ;

            }

            //ZF FLAG
            if(REG_XOR == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_XOR) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }


            PC+=2;
            break;
        }
        case (int) MOV_TYPES::ADRESS_TO_ADRESS:{
            quint16* REG1_P = &REG[OP1%4];
            quint16* REG2_P = &REG[OP2%4];


            quint16 RAM_VAL_1;
            if(*REG1_P % 2 == 0){
                RAM_VAL_1 = RAM[*REG1_P/2]>>8 | ( (RAM[*REG1_P/2]&0x00FF) << 8);
            }
            else{
                RAM_VAL_1 = (RAM[*REG1_P/2]&0xFF00 >> 8) | ((RAM[(*REG1_P/2)+1]) >> 8<<8);
            }

            quint16 RAM_VAL_2;
            if(*REG2_P % 2 == 0){
                RAM_VAL_2 = RAM[*REG2_P/2]>>8 | ( (RAM[*REG2_P/2]&0x00FF) << 8);
            }
            else{
                RAM_VAL_2 = (RAM[*REG2_P/2]&0xFF00 >> 8) | ((RAM[(*REG2_P/2)+1]) >> 8<<8);
            }

            quint16 REG_XOR = RAM_VAL_1 ^ RAM_VAL_2;

            if(*REG2_P % 2 == 0){
                RAM[*REG2_P/2] = REG_XOR<<8 | ( (REG_XOR>>8));
            }
            else{
                //qDebug()<<*REG2_P/2;
                RAM[*REG2_P/2] = (RAM[*REG2_P/2] & 0xFF00) | (REG_XOR & 0x00FF);

                RAM[*REG2_P/2 + 1] = (RAM[*REG2_P/2+1] & 0x00FF ) | (REG_XOR & 0xFF00) ;

            }

            //ZF FLAG
            if(REG_XOR == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((REG_XOR) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }


            break;
        }



        }


        break;
    }
    ///NOT
    case NOT:{
        qDebug()<<"NOT";
        quint8 OP = (OP_CODE & 0xF000)>>12;


        quint8 TYPE = getOPType_single(OP);

        switch (TYPE) {
        case (int)MOV_TYPES_S::REGISTER:{
            qDebug()<<"TO REGISTER";
            quint16* REG_P = &REG[OP];
            *REG_P = ~*REG_P;

            //ZF FLAG
            if(*REG_P == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((*REG_P) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            break;
        }
        case (int)MOV_TYPES_S::ADRESS:{
            qDebug()<<"TO ADRESS";
            quint16* REG_P = &REG[OP%4];

            if(*REG_P % 2 == 0){
                RAM[*REG_P/2] = ~RAM[*REG_P/2];

                //ZF FLAG
                if(RAM[*REG_P/2] == 0){
                    PS = PS | 0b0000000000000010;
                }
                else{
                    PS = PS & ~(0b0000000000000010);
                }
                //NF FLAG
                if ((RAM[*REG_P/2]) >> 0xF == 1){
                    PS = PS | 0b0000000000000001;
                }
                else{
                    PS = PS & ~(0b0000000000000001);
                }


            }
            else{

                RAM[*REG_P/2] = (RAM[*REG_P/2] & 0xFF00) | (~(RAM[*REG_P/2])& 0x00FF);
                RAM[*REG_P/2+1] = (~(RAM[*REG_P/2+1])& 0xFF00) | (RAM[*REG_P/2+1]&0x00FF);

                //ZF FLAG
                if( ( (RAM[*REG_P/2]&0xFF00 >> 8) | ((RAM[(*REG_P/2)+1]) >> 8<<8) ) == 0){
                    PS = PS | 0b0000000000000010;
                }
                else{
                    PS = PS & ~(0b0000000000000010);
                }
                //NF FLAG
                if (((RAM[*REG_P/2]&0xFF00 >> 8) | ((RAM[(*REG_P/2)+1]) >> 8<<8)) >> 0xF == 1){
                    PS = PS | 0b0000000000000001;
                }
                else{
                    PS = PS & ~(0b0000000000000001);
                }

            }

            break;
        }

        }

        break;

    }

    ///SHL
    case SHL:{
        qDebug()<<"SHL";
        quint8 NUMBER = ((OP_CODE & 0xF000)>>12) +1;

        quint8 OP = (OP_CODE & 0x0F00)>>8;


        quint8 TYPE = getOPType_single(OP);

        switch (TYPE) {
        case (int) MOV_TYPES_S::REGISTER:{
            qDebug()<<"TO REGISTER";
            quint16* REG_P = &REG[OP];
            *REG_P = *REG_P << NUMBER;

            //ZF FLAG
            if(*REG_P == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((*REG_P) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            break;
        }
        case (int) MOV_TYPES_S::ADRESS:{
            qDebug()<<"TO REGISTER";
            quint16* REG_P = &REG[OP%4];
            quint16 RAM_VAL;
            if(*REG_P%2 == 0){
               RAM_VAL = RAM[*REG_P/2]>>8 | ( (RAM[*REG_P/2]&0x00FF) << 8);
               RAM_VAL = RAM_VAL << NUMBER;
               RAM[*REG_P/2] = RAM_VAL<<8 | ( (RAM_VAL>>8));

               //ZF FLAG
               if(RAM_VAL == 0){
                   PS = PS | 0b0000000000000010;
               }
               else{
                   PS = PS & ~(0b0000000000000010);
               }
               //NF FLAG
               if ((RAM_VAL) >> 0xF == 1){
                   PS = PS | 0b0000000000000001;
               }
               else{
                   PS = PS & ~(0b0000000000000001);
               }
            }
            else{
               RAM_VAL = (RAM[*REG_P/2]&0xFF00 >> 8) | ((RAM[(*REG_P/2)+1]) >> 8<<8);
               RAM_VAL = RAM_VAL << NUMBER;
               RAM[*REG_P/2] = (RAM[*REG_P/2] & 0xFF00) | (RAM_VAL & 0x00FF);
               RAM[*REG_P/2 + 1] = (RAM[*REG_P/2+1] & 0x00FF ) | (RAM_VAL & 0xFF00) ;

               //ZF FLAG
               if(RAM_VAL == 0){
                   PS = PS | 0b0000000000000010;
               }
               else{
                   PS = PS & ~(0b0000000000000010);
               }
               //NF FLAG
               if ((RAM_VAL) >> 0xF == 1){
                   PS = PS | 0b0000000000000001;
               }
               else{
                   PS = PS & ~(0b0000000000000001);
               }

            }

            break;
        }
        }
        break;
    }

    ///SHR
    case SHR:{
        qDebug()<<"SHR";
        quint8 NUMBER = ((OP_CODE & 0xF000)>>12) +1;

        quint8 OP = (OP_CODE & 0x0F00)>>8;


        quint8 TYPE = getOPType_single(OP);

        switch (TYPE) {
        case (int) MOV_TYPES_S::REGISTER:{
            qDebug()<<"TO REGISTER";
            quint16* REG_P = &REG[OP];
            *REG_P = *REG_P >> NUMBER;

            //ZF FLAG
            if(*REG_P == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((*REG_P) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }


            break;
        }
        case (int) MOV_TYPES_S::ADRESS:{
            qDebug()<<"TO REGISTER";
            quint16* REG_P = &REG[OP%4];
            quint16 RAM_VAL;
            if(*REG_P%2 == 0){
               RAM_VAL = RAM[*REG_P/2]>>8 | ( (RAM[*REG_P/2]&0x00FF) << 8);
               RAM_VAL = RAM_VAL >> NUMBER;
               RAM[*REG_P/2] = RAM_VAL<<8 | ( (RAM_VAL>>8));

               //ZF FLAG
               if(RAM_VAL == 0){
                   PS = PS | 0b0000000000000010;
               }
               else{
                   PS = PS & ~(0b0000000000000010);
               }
               //NF FLAG
               if ((RAM_VAL) >> 0xF == 1){
                   PS = PS | 0b0000000000000001;
               }
               else{
                   PS = PS & ~(0b0000000000000001);
               }

            }
            else{
               RAM_VAL = (RAM[*REG_P/2]&0xFF00 >> 8) | ((RAM[(*REG_P/2)+1]) >> 8<<8);
               RAM_VAL = RAM_VAL >> NUMBER;
               RAM[*REG_P/2] = (RAM[*REG_P/2] & 0xFF00) | (RAM_VAL & 0x00FF);
               RAM[*REG_P/2 + 1] = (RAM[*REG_P/2+1] & 0x00FF ) | (RAM_VAL & 0xFF00);


               //ZF FLAG
               if(RAM_VAL == 0){
                   PS = PS | 0b0000000000000010;
               }
               else{
                   PS = PS & ~(0b0000000000000010);
               }
               //NF FLAG
               if ((RAM_VAL) >> 0xF == 1){
                   PS = PS | 0b0000000000000001;
               }
               else{
                   PS = PS & ~(0b0000000000000001);
               }


            }

            break;
        }
        }
        break;
    }
    ///SAR
    case SAR:{
        qDebug()<<"SAR";
        quint8 NUMBER = ((OP_CODE & 0xF000)>>12) +1;

        quint8 OP = (OP_CODE & 0x0F00)>>8;


        quint8 TYPE = getOPType_single(OP);

        switch (TYPE) {
        case (int) MOV_TYPES_S::REGISTER:{
            qDebug()<<"TO REGISTER";
            quint16* REG_P = &REG[OP];
            quint16 MASK = *REG_P & 0x1000000000000000b ? (0xFFFF << (16-NUMBER)):0;
            *REG_P = (*REG_P >> NUMBER) | MASK;


            //ZF FLAG
            if(*REG_P == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((*REG_P) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            break;
        }
        case (int) MOV_TYPES_S::ADRESS:{
            qDebug()<<"TO REGISTER";
            quint16* REG_P = &REG[OP%4];
            quint16 RAM_VAL;

            if(*REG_P%2 == 0){
               RAM_VAL = RAM[*REG_P/2]>>8 | ( (RAM[*REG_P/2]&0x00FF) << 8);
               quint16 MASK = RAM_VAL & 0x1000000000000000b ? (0xFFFF << (16-NUMBER)):0;
               RAM_VAL = (RAM_VAL >> NUMBER) | MASK;
               RAM[*REG_P/2] = RAM_VAL<<8 | ( (RAM_VAL>>8));

               //ZF FLAG
               if(RAM_VAL == 0){
                   PS = PS | 0b0000000000000010;
               }
               else{
                   PS = PS & ~(0b0000000000000010);
               }
               //NF FLAG
               if ((RAM_VAL) >> 0xF == 1){
                   PS = PS | 0b0000000000000001;
               }
               else{
                   PS = PS & ~(0b0000000000000001);
               }

            }
            else{
               RAM_VAL = (RAM[*REG_P/2]&0xFF00 >> 8) | ((RAM[(*REG_P/2)+1]) >> 8<<8);
               quint16 MASK = RAM_VAL & 0x1000000000000000b ? (0xFFFF << (16-NUMBER)):0;
               RAM_VAL = (RAM_VAL >> NUMBER) | MASK;
               RAM[*REG_P/2] = (RAM[*REG_P/2] & 0xFF00) | (RAM_VAL & 0x00FF);
               RAM[*REG_P/2 + 1] = (RAM[*REG_P/2+1] & 0x00FF ) | (RAM_VAL & 0xFF00) ;


               //ZF FLAG
               if(RAM_VAL == 0){
                   PS = PS | 0b0000000000000010;
               }
               else{
                   PS = PS & ~(0b0000000000000010);
               }
               //NF FLAG
               if ((RAM_VAL) >> 0xF == 1){
                   PS = PS | 0b0000000000000001;
               }
               else{
                   PS = PS & ~(0b0000000000000001);
               }

            }

            break;
        }
        }
        break;
    }

    ///ROL
    case ROL:{
        qDebug()<<"ROL";
        quint8 NUMBER = ((OP_CODE & 0xF000)>>12) +1;

        quint8 OP = (OP_CODE & 0x0F00)>>8;


        quint8 TYPE = getOPType_single(OP);

        switch (TYPE) {
        case (int) MOV_TYPES_S::REGISTER:{
            qDebug()<<"TO REGISTER";
            quint16* REG_P = &REG[OP];

            *REG_P = (*REG_P << NUMBER) | (*REG_P >> (16 - NUMBER));


            //ZF FLAG
            if(*REG_P == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((*REG_P) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            break;
        }
        case (int) MOV_TYPES_S::ADRESS:{
            qDebug()<<"TO ADRESS";
            quint16* REG_P = &REG[OP%4];
            quint16 RAM_VAL;
            if(*REG_P%2 == 0){
               RAM_VAL = RAM[*REG_P/2]>>8 | ( (RAM[*REG_P/2]&0x00FF) << 8);
               RAM_VAL = (RAM_VAL << NUMBER) | (RAM_VAL >> (16 - NUMBER));
               RAM[*REG_P/2] = RAM_VAL<<8 | ( (RAM_VAL>>8));

               //ZF FLAG
               if(RAM_VAL == 0){
                   PS = PS | 0b0000000000000010;
               }
               else{
                   PS = PS & ~(0b0000000000000010);
               }
               //NF FLAG
               if ((RAM_VAL) >> 0xF == 1){
                   PS = PS | 0b0000000000000001;
               }
               else{
                   PS = PS & ~(0b0000000000000001);
               }

            }
            else{
               RAM_VAL = (RAM[*REG_P/2]&0xFF00 >> 8) | ((RAM[(*REG_P/2)+1]) >> 8<<8);
               RAM_VAL = (RAM_VAL << NUMBER) | (RAM_VAL >> (16 - NUMBER));
               RAM[*REG_P/2] = (RAM[*REG_P/2] & 0xFF00) | (RAM_VAL & 0x00FF);
               RAM[*REG_P/2 + 1] = (RAM[*REG_P/2+1] & 0x00FF ) | (RAM_VAL & 0xFF00) ;


               //ZF FLAG
               if(RAM_VAL == 0){
                   PS = PS | 0b0000000000000010;
               }
               else{
                   PS = PS & ~(0b0000000000000010);
               }
               //NF FLAG
               if ((RAM_VAL) >> 0xF == 1){
                   PS = PS | 0b0000000000000001;
               }
               else{
                   PS = PS & ~(0b0000000000000001);
               }


            }

            break;
        }

        }
        break;
    }

    ///ROR
    case ROR:{
        qDebug()<<"ROR";
        quint8 NUMBER = ((OP_CODE & 0xF000)>>12) +1;

        quint8 OP = (OP_CODE & 0x0F00)>>8;


        quint8 TYPE = getOPType_single(OP);

        switch (TYPE) {
        case (int) MOV_TYPES_S::REGISTER:{
            qDebug()<<"TO REGISTER";
            quint16* REG_P = &REG[OP];

            *REG_P = (*REG_P >> NUMBER) | (*REG_P << (16 - NUMBER));

            //ZF FLAG
            if(*REG_P == 0){
                PS = PS | 0b0000000000000010;
            }
            else{
                PS = PS & ~(0b0000000000000010);
            }
            //NF FLAG
            if ((*REG_P) >> 0xF == 1){
                PS = PS | 0b0000000000000001;
            }
            else{
                PS = PS & ~(0b0000000000000001);
            }

            break;
        }
        case (int) MOV_TYPES_S::ADRESS:{
            qDebug()<<"TO ADRESS";
            quint16* REG_P = &REG[OP%4];
            quint16 RAM_VAL;
            if(*REG_P%2 == 0){
               RAM_VAL = RAM[*REG_P/2]>>8 | ( (RAM[*REG_P/2]&0x00FF) << 8);
               RAM_VAL = (RAM_VAL >> NUMBER) | (RAM_VAL << (16 - NUMBER));
               RAM[*REG_P/2] = RAM_VAL<<8 | ( (RAM_VAL>>8));

               //ZF FLAG
               if(RAM_VAL == 0){
                   PS = PS | 0b0000000000000010;
               }
               else{
                   PS = PS & ~(0b0000000000000010);
               }
               //NF FLAG
               if ((RAM_VAL) >> 0xF == 1){
                   PS = PS | 0b0000000000000001;
               }
               else{
                   PS = PS & ~(0b0000000000000001);
               }

            }
            else{
               RAM_VAL = (RAM[*REG_P/2]&0xFF00 >> 8) | ((RAM[(*REG_P/2)+1]) >> 8<<8);
               RAM_VAL = (RAM_VAL >> NUMBER) | (RAM_VAL << (16 - NUMBER));
               RAM[*REG_P/2] = (RAM[*REG_P/2] & 0xFF00) | (RAM_VAL & 0x00FF);
               RAM[*REG_P/2 + 1] = (RAM[*REG_P/2+1] & 0x00FF ) | (RAM_VAL & 0xFF00) ;


               //ZF FLAG
               if(RAM_VAL == 0){
                   PS = PS | 0b0000000000000010;
               }
               else{
                   PS = PS & ~(0b0000000000000010);
               }
               //NF FLAG
               if ((RAM_VAL) >> 0xF == 1){
                   PS = PS | 0b0000000000000001;
               }
               else{
                   PS = PS & ~(0b0000000000000001);
               }


            }

            break;
        }
        }
        break;
    }
    ///RCL
    case RCL:{
        qDebug()<<"RCL";
        quint8 NUMBER = ((OP_CODE & 0xF000)>>12) +1;

        quint8 OP = (OP_CODE & 0x0F00)>>8;


        quint8 TYPE = getOPType_single(OP);

        switch (TYPE) {
        case (int) MOV_TYPES_S::REGISTER:{
            qDebug()<<"TO REGISTER";
            quint16* REG_P = &REG[OP];


            quint32 REG_32 = *REG_P;
            REG_32 = (REG_32 << 1) | ((PS & 0b0000000000000100) == 0 ? 0: 0b00000000000000001);
            REG_32 = (((REG_32 << NUMBER) | (REG_32 >> (17 - NUMBER)))&0x1FFFF)>>1;
            quint16 REG_16 = REG_32 & 0xFFFF;
            *REG_P = REG_16;


            break;
        }
        case (int) MOV_TYPES_S::ADRESS:{
            qDebug()<<"TO ADRESS";
            quint16* REG_P = &REG[OP%4];
            quint16 RAM_VAL;
            if(*REG_P%2 == 0){
               RAM_VAL = RAM[*REG_P/2]>>8 | ( (RAM[*REG_P/2]&0x00FF) << 8);
               quint32 REG_32 = *REG_P;
               REG_32 = (REG_32 << 1) |  ((PS & 0b0000000000000100) == 0 ? 0: 0b00000000000000001);
               REG_32 = (((REG_32 << NUMBER) | (REG_32 >> (17 - NUMBER)))&0x1FFFF)>>1;
               quint16 REG_16 = REG_32 & 0xFFFF;
               RAM_VAL = REG_16;
               RAM[*REG_P/2] = RAM_VAL<<8 | ( (RAM_VAL>>8));
            }
            else{
               RAM_VAL = (RAM[*REG_P/2]&0xFF00 >> 8) | ((RAM[(*REG_P/2)+1]) >> 8<<8);
               quint32 REG_32 = *REG_P;
               REG_32 = (REG_32 << 1) |  ((PS & 0b0000000000000100) == 0 ? 0: 0b00000000000000001);
               REG_32 = (((REG_32 << NUMBER) | (REG_32 >> (17 - NUMBER)))&0x1FFFF)>>1;
               quint16 REG_16 = REG_32 & 0xFFFF;
               RAM_VAL = REG_16;
               RAM[*REG_P/2] = (RAM[*REG_P/2] & 0xFF00) | (RAM_VAL & 0x00FF);
               RAM[*REG_P/2 + 1] = (RAM[*REG_P/2+1] & 0x00FF ) | (RAM_VAL & 0xFF00) ;

            }

            break;
        }
        }
        break;

    }

    ///RCR
    case RCR:{
        qDebug()<<"RCR";
        quint8 NUMBER = ((OP_CODE & 0xF000)>>12) +1;

        quint8 OP = (OP_CODE & 0x0F00)>>8;


        quint8 TYPE = getOPType_single(OP);

        switch (TYPE) {
        case (int) MOV_TYPES_S::REGISTER:{
            qDebug()<<"TO REGISTER";
            quint16* REG_P = &REG[OP];


            quint32 REG_32 = *REG_P;
            REG_32 = (REG_32 << 1) | ((PS & 0b0000000000000100) == 0 ? 0: 0b00000000000000001);
            REG_32 = (((REG_32 >> NUMBER) | (REG_32 << (17 - NUMBER)))&0x1FFFF)>>1;
            quint16 REG_16 = REG_32 & 0xFFFF;
            *REG_P = REG_16;


            break;
        }
        case (int) MOV_TYPES_S::ADRESS:{
            qDebug()<<"TO ADRESS";
            quint16* REG_P = &REG[OP%4];
            quint16 RAM_VAL;
            if(*REG_P%2 == 0){
               RAM_VAL = RAM[*REG_P/2]>>8 | ( (RAM[*REG_P/2]&0x00FF) << 8);
               quint32 REG_32 = *REG_P;
               REG_32 = (REG_32 << 1) |  ((PS & 0b0000000000000100) == 0 ? 0: 0b00000000000000001);
               REG_32 = (((REG_32 >> NUMBER) | (REG_32 << (17 - NUMBER)))&0x1FFFF)>>1;
               quint16 REG_16 = REG_32 & 0xFFFF;
               RAM_VAL = REG_16;
               RAM[*REG_P/2] = RAM_VAL<<8 | ( (RAM_VAL>>8));
            }
            else{
               RAM_VAL = (RAM[*REG_P/2]&0xFF00 >> 8) | ((RAM[(*REG_P/2)+1]) >> 8<<8);
               quint32 REG_32 = *REG_P;
               REG_32 = (REG_32 << 1) |  ((PS & 0b0000000000000100) == 0 ? 0: 0b00000000000000001);
               REG_32 = (((REG_32 >> NUMBER) | (REG_32 << (17 - NUMBER)))&0x1FFFF)>>1;
               quint16 REG_16 = REG_32 & 0xFFFF;
               RAM_VAL = REG_16;
               RAM[*REG_P/2] = (RAM[*REG_P/2] & 0xFF00) | (RAM_VAL & 0x00FF);
               RAM[*REG_P/2 + 1] = (RAM[*REG_P/2+1] & 0x00FF ) | (RAM_VAL & 0xFF00) ;

            }

            break;
        }
        }

        break;
    }

    ///PUSH
    case PUSH:{
        qDebug()<<"PUSH";

        quint8 OP = (OP_CODE & 0xF000)>>12;
        quint8 TYPE = getOPType_single(OP);
        switch (TYPE) {
        case (int)MOV_TYPES_S::REGISTER:{
            quint16* REG_P = &REG[OP];
            RAM[SP/2-1] = *REG_P;
            SP-=2;

            break;
        }
        case (int) MOV_TYPES_S::ADRESS:{
            quint16* REG_P = &REG[OP%4];
            quint16 RAM_VAL;
            if(*REG_P % 2 == 0){
                RAM_VAL = RAM[*REG_P/2]>>8 | ( (RAM[*REG_P/2]&0x00FF) << 8);
            }
            else{

                RAM_VAL = (RAM[*REG_P/2]&0xFF00 >> 8) | ((RAM[(*REG_P/2)+1]) >> 8<<8);

            }
            RAM[SP/2-1] = RAM_VAL;
            SP-=2;
            break;
        }

        }


        break;
    }
    ///POP
    case POP:{
        qDebug()<<"POP";
        quint8 OP = (OP_CODE & 0xF000)>>12;
        quint8 TYPE = getOPType_single(OP);
        switch (TYPE) {
        case (int)MOV_TYPES_S::REGISTER:{
            quint16* REG_P = &REG[OP];
            *REG_P = RAM[(SP+2)/2-1];
            qDebug()<<SP/2-1<<" IS "<<RAM[(SP+2)/2-1];
            //RAM[SP/2-1] = 0;
            SP+=2;

            break;
        }
        case (int) MOV_TYPES_S::ADRESS:{
            quint16* REG_P = &REG[OP%4];
            if(*REG_P % 2 == 0){
                RAM[*REG_P/2] = RAM[(SP+2)/2-1];
                //RAM[SP/2-1] = 0;
                SP+=2;
            }
            else{
                //qDebug()<<*REG2_P/2;
                RAM[*REG_P/2] = (RAM[(SP+2)/2-1] >> 8);

                RAM[*REG_P/2 + 1] = (RAM[(SP+2)/2-1] & 0x00FF)<<8;
                //RAM[SP/2-1] = 0;
                SP+=2;

            }

            break;
        }

        }
        break;
    }

    ///JMP
    case JMP :{
    qDebug()<<"JMP";
    quint16 ADRESS = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]<<8));
    PC = ADRESS/2;

    break;
    }
    ///JGE
    case JGE :{
    qDebug()<<"JGE";
    quint16 ADRESS = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]<<8));

    if(((PS & 0b0000000000000010)>>1) ==1 || (PS & 0b0000000000000001) ==0){
        PC = ADRESS/2;

    }
    else PC+=2;
    break;
    }
    ///JL
    case JL  :{
    qDebug()<<"JL";
    quint16 ADRESS = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]<<8));

    if( (PS & 0b0000000000000001) ==1){
           PC = ADRESS/2;

       }
       else PC+=2;

       break;
    }
     ///JNZ
     case JNZ :{
     qDebug()<<"JNZ";
     quint16 ADRESS = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]<<8));
     qDebug()<<"ADRESS: "<<QString::number(ADRESS,16);
     qDebug()<<"PS"<<QString::number(PS, 2);
     qDebug()<<"CHECK BIT"<<QString::number(PS & 0b0000000000000010, 2);
     if(((PS & 0b0000000000000010)>>1) ==0){
         PC = ADRESS/2;
         qDebug()<<"PC"<<QString::number(PC, 16);
     }
     else PC+=2;

     break;
    }
     ///JZ
     case JZ  :{
     qDebug()<<"JZ";
     quint16 ADRESS = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]<<8));

     if(( (PS & 0b0000000000000010)>>1 ) == 1){
         PC = ADRESS/2;

     }
     else PC+=2;
     break;
    }
     ///JLE
     case JLE :{
     qDebug()<<"JLE";
     quint16 ADRESS = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]<<8));

     if(((PS & 0b0000000000000010)>>1) ==1 || (PS & 0b0000000000000001) ==1){
         PC = ADRESS/2;

     }
     else PC+=2;
     break;

    }
     ///JG
     case JG  :{
     qDebug()<<"JG";
     quint16 ADRESS = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]<<8));

     if( (PS & 0b0000000000000001) ==0){
         PC = ADRESS/2;

     }
     else PC+=2;
     break;

    }

    case PIXEL:{
        qDebug()<<"PIXEL";
        quint16 XY = RAM[IR/2+1]>>8 | ( (RAM[IR/2+1]<<8));
        quint16 COLOR16 = RAM[IR/2+2]>>8 | ( (RAM[IR/2+2]<<8));

        emit colorPixel(XY, COLOR16);
        PC+=4;


        break;
    }



     if( PC >= 64){
         *isRunning = false;
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

quint8 Processor::getOPType_single(quint8 OP){
    if(OP>=0 && OP <= 3) return (quint8)MOV_TYPES_S::REGISTER;
    if(OP >= 4 && OP <= 7) return (quint8) MOV_TYPES_S::ADRESS;
    return (quint8) MOV_TYPES_S::UNDEFINED;
}



void Processor::reset(){
    REG[0] = 0x0002;
    REG[1] = 0x0008;
    REG[2] = 0x0004;
    REG[3] = 0x0000;

    PC = 0;
    IR = 0;
    PS = 0;
    for(int i = 0; i < 128; i++){
        RAM[i] = 0;
    }
    //memset(&RAM, 128, 2);
    emit dataChanged();
}

void Processor::resetCommand(){
    PC = 0;
    IR = 0;
    PS = 0;
    SP = 256;
    emit dataChanged();
}





