#include "debugger.h"


Debugger::Debugger()
{

}


void Debugger::update(Processor * CPU, QListWidget* LIST){
    LIST->clear();
    bool IS_NUMBER = false;
    bool IS_NUMBER_PIXEL = false;
    for(int i = 0; i < 128; i++){
        quint16 OP_CODE_LB = CPU->RAM[i]>>8 | ( (CPU->RAM[i]&0x00FF)<<8);
        quint16 OP_CODE_BL = CPU->RAM[i];
        QString COMMAND = "";

        if(IS_NUMBER == false){
            switch ((OP_CODE_LB&0xFF00)>>8) {
        case NOP:{
            COMMAND+="NOP";
            break;
        }
        case STOP:{
            COMMAND+="STOP";
            break;
        }

        case MOV:{
            COMMAND+="MOV";
            quint8 OP1 = (OP_CODE_BL & 0xF000)>>12;
            quint8 OP2 = (OP_CODE_BL & 0x0F00)>>8;
            int TYPE = getOPType(OP1, OP2);
            switch (TYPE) {
            case (int)MOV_TYPES::REGISTER_TO_REGISTER:{
                QString REG1 = "R"+QString::number(OP1);
                QString REG2 = "R"+QString::number(OP2);
                COMMAND += " " + REG1 + ", " + REG2;
                break;
            }
            case (int)MOV_TYPES::NUMBER_TO_REGISTER:{
                QString REG2 = "R"+QString::number(OP2);
                QString NUMBER = QString::number(
                            ((CPU->RAM[i+1] >> 8) | ((CPU->RAM[i+1]&0x00FF)<<8))
                        ,16)
                        .rightJustified(4,'0',true);
                COMMAND += " " + NUMBER + ", " + REG2;
                IS_NUMBER = true;
            break;

            }

            case (int)MOV_TYPES::ADRESS_TO_REGISTER:{
                QString REG1;
                REG1.append("(R").append(QString::number(OP1%4)).append(")");
                QString REG2 = "R"+QString::number(OP2);
                COMMAND += " " + REG1 + ", " + REG2;
                break;
            }

            case (int)MOV_TYPES::REGISTER_TO_ADRESS:{
                QString REG1 = "R"+QString::number(OP1);

                QString REG2;
                REG2.append("(R").append(QString::number(OP2%4)).append(")");
                COMMAND += " " + REG1 + ", " + REG2;
                break;
            }

            case (int) MOV_TYPES::NUMBER_TO_ADRESS:{
            QString NUMBER = QString::number(
                        ((CPU->RAM[i+1] >> 8) | ((CPU->RAM[i+1]&0x00FF)<<8))
                    ,16)
                    .rightJustified(4,'0',true);
            QString REG2;
            REG2.append("(R").append(QString::number(OP2%4)).append(")");
            COMMAND += " " + NUMBER + ", " + REG2;
            IS_NUMBER = true;
            break;
            }

            case (int) MOV_TYPES::ADRESS_TO_ADRESS:{
                QString REG1;
                REG1.append("(R").append(QString::number(OP1%4)).append(")");
                QString REG2;
                REG2.append("(R").append(QString::number(OP2%4)).append(")");
                COMMAND += " " + REG2 + ", " + REG2;
            }

            }
            break;
        }

        case ADD:{
            COMMAND += "ADD";
            quint8 OP1 = (OP_CODE_BL & 0xF000)>>12;
            quint8 OP2 = (OP_CODE_BL & 0x0F00)>>8;
            int TYPE = getOPType(OP1, OP2);
            switch (TYPE) {
            case (int)MOV_TYPES::REGISTER_TO_REGISTER:{
                QString REG1 = "R"+QString::number(OP1);
                QString REG2 = "R"+QString::number(OP2);
                COMMAND += " " + REG1 + ", " + REG2;
                break;
            }
            case (int)MOV_TYPES::NUMBER_TO_REGISTER:{
                QString REG2 = "R"+QString::number(OP2);
                QString NUMBER = QString::number(
                            ((CPU->RAM[i+1] >> 8) | ((CPU->RAM[i+1]&0x00FF)<<8))
                        ,16)
                        .rightJustified(4,'0',true);
                COMMAND += " " + NUMBER + ", " + REG2;
                IS_NUMBER = true;
            break;

            }

            case (int)MOV_TYPES::ADRESS_TO_REGISTER:{
                QString REG1;
                REG1.append("(R").append(QString::number(OP1%4)).append(")");
                QString REG2 = "R"+QString::number(OP2);
                COMMAND += " " + REG1 + ", " + REG2;
                break;
            }

            case (int)MOV_TYPES::REGISTER_TO_ADRESS:{
                QString REG1 = "R"+QString::number(OP1);

                QString REG2;
                REG2.append("(R").append(QString::number(OP2%4)).append(")");
                COMMAND += " " + REG1 + ", " + REG2;
                break;
            }

            case (int) MOV_TYPES::NUMBER_TO_ADRESS:{
            QString NUMBER = QString::number(
                        ((CPU->RAM[i+1] >> 8) | ((CPU->RAM[i+1]&0x00FF)<<8))
                    ,16)
                    .rightJustified(4,'0',true);
            QString REG2;
            REG2.append("(R").append(QString::number(OP2%4)).append(")");
            COMMAND += " " + NUMBER + ", " + REG2;
            IS_NUMBER = true;
            break;
            }

            case (int) MOV_TYPES::ADRESS_TO_ADRESS:{
                QString REG1;
                REG1.append("(R").append(QString::number(OP1%4)).append(")");
                QString REG2;
                REG2.append("(R").append(QString::number(OP2%4)).append(")");
                COMMAND += " " + REG2 + ", " + REG2;
            }

            }
            break;
        }

        case SUB:{
            COMMAND += "SUB";
            quint8 OP1 = (OP_CODE_BL & 0xF000)>>12;
            quint8 OP2 = (OP_CODE_BL & 0x0F00)>>8;
            int TYPE = getOPType(OP1, OP2);
            switch (TYPE) {
            case (int)MOV_TYPES::REGISTER_TO_REGISTER:{
                QString REG1 = "R"+QString::number(OP1);
                QString REG2 = "R"+QString::number(OP2);
                COMMAND += " " + REG1 + ", " + REG2;
                break;
            }
            case (int)MOV_TYPES::NUMBER_TO_REGISTER:{
                QString REG2 = "R"+QString::number(OP2);
                QString NUMBER = QString::number(
                            ((CPU->RAM[i+1] >> 8) | ((CPU->RAM[i+1]&0x00FF)<<8))
                        ,16)
                        .rightJustified(4,'0',true);
                COMMAND += " " + NUMBER + ", " + REG2;
                IS_NUMBER = true;
            break;

            }

            case (int)MOV_TYPES::ADRESS_TO_REGISTER:{
                QString REG1;
                REG1.append("(R").append(QString::number(OP1%4)).append(")");
                QString REG2 = "R"+QString::number(OP2);
                COMMAND += " " + REG1 + ", " + REG2;
                break;
            }

            case (int)MOV_TYPES::REGISTER_TO_ADRESS:{
                QString REG1 = "R"+QString::number(OP1);

                QString REG2;
                REG2.append("(R").append(QString::number(OP2%4)).append(")");
                COMMAND += " " + REG1 + ", " + REG2;
                break;
            }

            case (int) MOV_TYPES::NUMBER_TO_ADRESS:{
            QString NUMBER = QString::number(
                        ((CPU->RAM[i+1] >> 8) | ((CPU->RAM[i+1]&0x00FF)<<8))
                    ,16)
                    .rightJustified(4,'0',true);
            QString REG2;
            REG2.append("(R").append(QString::number(OP2%4)).append(")");
            COMMAND += " " + NUMBER + ", " + REG2;
            IS_NUMBER = true;
            break;
            }

            case (int) MOV_TYPES::ADRESS_TO_ADRESS:{
                QString REG1;
                REG1.append("(R").append(QString::number(OP1%4)).append(")");
                QString REG2;
                REG2.append("(R").append(QString::number(OP2%4)).append(")");
                COMMAND += " " + REG2 + ", " + REG2;
            }

            }
            break;
        }

        case MUL:{
            COMMAND += "MUL";
            quint8 OP1 = (OP_CODE_BL & 0xF000)>>12;
            quint8 OP2 = (OP_CODE_BL & 0x0F00)>>8;
            int TYPE = getOPType(OP1, OP2);
            switch (TYPE) {
            case (int)MOV_TYPES::REGISTER_TO_REGISTER:{
                QString REG1 = "R"+QString::number(OP1);
                QString REG2 = "R"+QString::number(OP2);
                COMMAND += " " + REG1 + ", " + REG2;
                break;
            }
            case (int)MOV_TYPES::NUMBER_TO_REGISTER:{
                QString REG2 = "R"+QString::number(OP2);
                QString NUMBER = QString::number(
                            ((CPU->RAM[i+1] >> 8) | ((CPU->RAM[i+1]&0x00FF)<<8))
                        ,16)
                        .rightJustified(4,'0',true);
                COMMAND += " " + NUMBER + ", " + REG2;
                IS_NUMBER = true;
            break;

            }

            case (int)MOV_TYPES::ADRESS_TO_REGISTER:{
                QString REG1;
                REG1.append("(R").append(QString::number(OP1%4)).append(")");
                QString REG2 = "R"+QString::number(OP2);
                COMMAND += " " + REG1 + ", " + REG2;
                break;
            }

            case (int)MOV_TYPES::REGISTER_TO_ADRESS:{
                QString REG1 = "R"+QString::number(OP1);

                QString REG2;
                REG2.append("(R").append(QString::number(OP2%4)).append(")");
                COMMAND += " " + REG1 + ", " + REG2;
                break;
            }

            case (int) MOV_TYPES::NUMBER_TO_ADRESS:{
            QString NUMBER = QString::number(
                        ((CPU->RAM[i+1] >> 8) | ((CPU->RAM[i+1]&0x00FF)<<8))
                    ,16)
                    .rightJustified(4,'0',true);
            QString REG2;
            REG2.append("(R").append(QString::number(OP2%4)).append(")");
            COMMAND += " " + NUMBER + ", " + REG2;
            IS_NUMBER = true;
            break;
            }

            case (int) MOV_TYPES::ADRESS_TO_ADRESS:{
                QString REG1;
                REG1.append("(R").append(QString::number(OP1%4)).append(")");
                QString REG2;
                REG2.append("(R").append(QString::number(OP2%4)).append(")");
                COMMAND += " " + REG2 + ", " + REG2;
            }

            }
            break;
        }

        case DIV:{
            COMMAND += "DIV";
            quint8 OP1 = (OP_CODE_BL & 0xF000)>>12;
            quint8 OP2 = (OP_CODE_BL & 0x0F00)>>8;
            int TYPE = getOPType(OP1, OP2);
            switch (TYPE) {
            case (int)MOV_TYPES::REGISTER_TO_REGISTER:{
                QString REG1 = "R"+QString::number(OP1);
                QString REG2 = "R"+QString::number(OP2);
                COMMAND += " " + REG1 + ", " + REG2;
                break;
            }
            case (int)MOV_TYPES::NUMBER_TO_REGISTER:{
                QString REG2 = "R"+QString::number(OP2);
                QString NUMBER = QString::number(
                            ((CPU->RAM[i+1] >> 8) | ((CPU->RAM[i+1]&0x00FF)<<8))
                        ,16)
                        .rightJustified(4,'0',true);
                COMMAND += " " + NUMBER + ", " + REG2;
                IS_NUMBER = true;
            break;

            }

            case (int)MOV_TYPES::ADRESS_TO_REGISTER:{
                QString REG1;
                REG1.append("(R").append(QString::number(OP1%4)).append(")");
                QString REG2 = "R"+QString::number(OP2);
                COMMAND += " " + REG1 + ", " + REG2;
                break;
            }

            case (int)MOV_TYPES::REGISTER_TO_ADRESS:{
                QString REG1 = "R"+QString::number(OP1);

                QString REG2;
                REG2.append("(R").append(QString::number(OP2%4)).append(")");
                COMMAND += " " + REG1 + ", " + REG2;
                break;
            }

            case (int) MOV_TYPES::NUMBER_TO_ADRESS:{
            QString NUMBER = QString::number(
                        ((CPU->RAM[i+1] >> 8) | ((CPU->RAM[i+1]&0x00FF)<<8))
                    ,16)
                    .rightJustified(4,'0',true);
            QString REG2;
            REG2.append("(R").append(QString::number(OP2%4)).append(")");
            COMMAND += " " + NUMBER + ", " + REG2;
            IS_NUMBER = true;
            break;
            }

            case (int) MOV_TYPES::ADRESS_TO_ADRESS:{
                QString REG1;
                REG1.append("(R").append(QString::number(OP1%4)).append(")");
                QString REG2;
                REG2.append("(R").append(QString::number(OP2%4)).append(")");
                COMMAND += " " + REG2 + ", " + REG2;
            }

            }
            break;
        }

        case AND:{
            COMMAND += "AND";
            quint8 OP1 = (OP_CODE_BL & 0xF000)>>12;
            quint8 OP2 = (OP_CODE_BL & 0x0F00)>>8;
            int TYPE = getOPType(OP1, OP2);
            switch (TYPE) {
            case (int)MOV_TYPES::REGISTER_TO_REGISTER:{
                QString REG1 = "R"+QString::number(OP1);
                QString REG2 = "R"+QString::number(OP2);
                COMMAND += " " + REG1 + ", " + REG2;
                break;
            }
            case (int)MOV_TYPES::NUMBER_TO_REGISTER:{
                QString REG2 = "R"+QString::number(OP2);
                QString NUMBER = QString::number(
                            ((CPU->RAM[i+1] >> 8) | ((CPU->RAM[i+1]&0x00FF)<<8))
                        ,16)
                        .rightJustified(4,'0',true);
                COMMAND += " " + NUMBER + ", " + REG2;
                IS_NUMBER = true;
            break;

            }

            case (int)MOV_TYPES::ADRESS_TO_REGISTER:{
                QString REG1;
                REG1.append("(R").append(QString::number(OP1%4)).append(")");
                QString REG2 = "R"+QString::number(OP2);
                COMMAND += " " + REG1 + ", " + REG2;
                break;
            }

            case (int)MOV_TYPES::REGISTER_TO_ADRESS:{
                QString REG1 = "R"+QString::number(OP1);

                QString REG2;
                REG2.append("(R").append(QString::number(OP2%4)).append(")");
                COMMAND += " " + REG1 + ", " + REG2;
                break;
            }

            case (int) MOV_TYPES::NUMBER_TO_ADRESS:{
            QString NUMBER = QString::number(
                        ((CPU->RAM[i+1] >> 8) | ((CPU->RAM[i+1]&0x00FF)<<8))
                    ,16)
                    .rightJustified(4,'0',true);
            QString REG2;
            REG2.append("(R").append(QString::number(OP2%4)).append(")");
            COMMAND += " " + NUMBER + ", " + REG2;
            IS_NUMBER = true;
            break;
            }

            case (int) MOV_TYPES::ADRESS_TO_ADRESS:{
                QString REG1;
                REG1.append("(R").append(QString::number(OP1%4)).append(")");
                QString REG2;
                REG2.append("(R").append(QString::number(OP2%4)).append(")");
                COMMAND += " " + REG2 + ", " + REG2;
            }

            }
            break;
        }
        case OR:{
            COMMAND += "OR";
            quint8 OP1 = (OP_CODE_BL & 0xF000)>>12;
            quint8 OP2 = (OP_CODE_BL & 0x0F00)>>8;
            int TYPE = getOPType(OP1, OP2);
            switch (TYPE) {
            case (int)MOV_TYPES::REGISTER_TO_REGISTER:{
                QString REG1 = "R"+QString::number(OP1);
                QString REG2 = "R"+QString::number(OP2);
                COMMAND += " " + REG1 + ", " + REG2;
                break;
            }
            case (int)MOV_TYPES::NUMBER_TO_REGISTER:{
                QString REG2 = "R"+QString::number(OP2);
                QString NUMBER = QString::number(
                            ((CPU->RAM[i+1] >> 8) | ((CPU->RAM[i+1]&0x00FF)<<8))
                        ,16)
                        .rightJustified(4,'0',true);
                COMMAND += " " + NUMBER + ", " + REG2;
                IS_NUMBER = true;
            break;

            }

            case (int)MOV_TYPES::ADRESS_TO_REGISTER:{
                QString REG1;
                REG1.append("(R").append(QString::number(OP1%4)).append(")");
                QString REG2 = "R"+QString::number(OP2);
                COMMAND += " " + REG1 + ", " + REG2;
                break;
            }

            case (int)MOV_TYPES::REGISTER_TO_ADRESS:{
                QString REG1 = "R"+QString::number(OP1);

                QString REG2;
                REG2.append("(R").append(QString::number(OP2%4)).append(")");
                COMMAND += " " + REG1 + ", " + REG2;
                break;
            }

            case (int) MOV_TYPES::NUMBER_TO_ADRESS:{
            QString NUMBER = QString::number(
                        ((CPU->RAM[i+1] >> 8) | ((CPU->RAM[i+1]&0x00FF)<<8))
                    ,16)
                    .rightJustified(4,'0',true);
            QString REG2;
            REG2.append("(R").append(QString::number(OP2%4)).append(")");
            COMMAND += " " + NUMBER + ", " + REG2;
            IS_NUMBER = true;
            break;
            }

            case (int) MOV_TYPES::ADRESS_TO_ADRESS:{
                QString REG1;
                REG1.append("(R").append(QString::number(OP1%4)).append(")");
                QString REG2;
                REG2.append("(R").append(QString::number(OP2%4)).append(")");
                COMMAND += " " + REG2 + ", " + REG2;
            }

            }
            break;
        }
            case CMP:{
                COMMAND += "CMP";
                quint8 OP1 = (OP_CODE_BL & 0xF000)>>12;
                quint8 OP2 = (OP_CODE_BL & 0x0F00)>>8;
                int TYPE = getOPType(OP1, OP2);
                switch (TYPE) {
                case (int)MOV_TYPES::REGISTER_TO_REGISTER:{
                    QString REG1 = "R"+QString::number(OP1);
                    QString REG2 = "R"+QString::number(OP2);
                    COMMAND += " " + REG1 + ", " + REG2;
                    break;
                }
                case (int)MOV_TYPES::NUMBER_TO_REGISTER:{
                    QString REG2 = "R"+QString::number(OP2);
                    QString NUMBER = QString::number(
                                ((CPU->RAM[i+1] >> 8) | ((CPU->RAM[i+1]&0x00FF)<<8))
                            ,16)
                            .rightJustified(4,'0',true);
                    COMMAND += " " + NUMBER + ", " + REG2;
                    IS_NUMBER = true;
                break;

                }

                case (int)MOV_TYPES::ADRESS_TO_REGISTER:{
                    QString REG1;
                    REG1.append("(R").append(QString::number(OP1%4)).append(")");
                    QString REG2 = "R"+QString::number(OP2);
                    COMMAND += " " + REG1 + ", " + REG2;
                    break;
                }

                case (int)MOV_TYPES::REGISTER_TO_ADRESS:{
                    QString REG1 = "R"+QString::number(OP1);

                    QString REG2;
                    REG2.append("(R").append(QString::number(OP2%4)).append(")");
                    COMMAND += " " + REG1 + ", " + REG2;
                    break;
                }

                case (int) MOV_TYPES::NUMBER_TO_ADRESS:{
                QString NUMBER = QString::number(
                            ((CPU->RAM[i+1] >> 8) | ((CPU->RAM[i+1]&0x00FF)<<8))
                        ,16)
                        .rightJustified(4,'0',true);
                QString REG2;
                REG2.append("(R").append(QString::number(OP2%4)).append(")");
                COMMAND += " " + NUMBER + ", " + REG2;
                IS_NUMBER = true;
                break;
                }

                case (int) MOV_TYPES::ADRESS_TO_ADRESS:{
                    QString REG1;
                    REG1.append("(R").append(QString::number(OP1%4)).append(")");
                    QString REG2;
                    REG2.append("(R").append(QString::number(OP2%4)).append(")");
                    COMMAND += " " + REG2 + ", " + REG2;
                }

                }
                break;
            }

        case NOT:{
            COMMAND += "NOT";
            quint8 OP = (OP_CODE_BL & 0xF000)>>12;


            int TYPE = getOPType(OP);
            switch (TYPE) {
            case (int) MOV_TYPES_S::REGISTER:{
                QString REG = QString::number(OP);
                COMMAND+=" " + REG;
                break;
            }
            case (int) MOV_TYPES_S::ADRESS:{
                QString REG;
                REG.append("(R").append(QString::number(OP%4)).append(")");
                COMMAND+=" " + REG;
                break;
            }
            }
            break;
        }

        case PUSH:{
            COMMAND +="PUSH";
            quint8 OP = (OP_CODE_BL & 0xF000)>>12;


            int TYPE = getOPType(OP);
            switch (TYPE) {
            case (int) MOV_TYPES_S::REGISTER:{
                QString REG = QString::number(OP);
                COMMAND+=" " + REG;
                break;
            }
            case (int) MOV_TYPES_S::ADRESS:{
                QString REG;
                REG.append("(R").append(QString::number(OP%4)).append(")");
                COMMAND+=" " + REG;
                break;
            }
            }
            break;
        }

        case POP:{
            COMMAND+="POP";
            quint8 OP = (OP_CODE_BL & 0xF000)>>12;


            int TYPE = getOPType(OP);
            switch (TYPE) {
            case (int) MOV_TYPES_S::REGISTER:{
                QString REG = QString::number(OP);
                COMMAND+=" " + REG;
                break;
            }
            case (int) MOV_TYPES_S::ADRESS:{
                QString REG;
                REG.append("(R").append(QString::number(OP%4)).append(")");
                COMMAND+=" " + REG;
                break;
            }
            }
            break;
        }

        case SHL:{
            COMMAND +="SHL";
            quint8 NUMBER = ((OP_CODE_BL & 0xF000)>>12) +1;

            quint8 OP = (OP_CODE_BL & 0x0F00)>>8;


            quint8 TYPE = getOPType(OP);

            switch (TYPE) {
            case (int) MOV_TYPES_S::REGISTER:{
                QString REG = QString::number(OP);
                COMMAND+=" " + QString::number(NUMBER) + ", " + REG;
                break;
            }
            case (int) MOV_TYPES_S::ADRESS:{
                QString REG;
                REG.append("(R").append(QString::number(OP%4)).append(")");
                COMMAND+=" " + QString::number(NUMBER) + ", " + REG;
                break;
            }
            }


            break;
        }

        case SHR:{
            COMMAND +="SHR";
            quint8 NUMBER = ((OP_CODE_BL & 0xF000)>>12) +1;

            quint8 OP = (OP_CODE_BL & 0x0F00)>>8;


            quint8 TYPE = getOPType(OP);

            switch (TYPE) {
            case (int) MOV_TYPES_S::REGISTER:{
                QString REG = QString::number(OP);
                COMMAND+=" " + QString::number(NUMBER) + ", " + REG;
                break;
            }
            case (int) MOV_TYPES_S::ADRESS:{
                QString REG;
                REG.append("(R").append(QString::number(OP%4)).append(")");
                COMMAND+=" " + QString::number(NUMBER) + ", " + REG;
                break;
            }
            }
            break;
        }
        case SAR:{
            COMMAND +="SAR";
            quint8 NUMBER = ((OP_CODE_BL & 0xF000)>>12) +1;

            quint8 OP = (OP_CODE_BL & 0x0F00)>>8;


            quint8 TYPE = getOPType(OP);

            switch (TYPE) {
            case (int) MOV_TYPES_S::REGISTER:{
                QString REG = QString::number(OP);
                COMMAND+=" " + QString::number(NUMBER) + ", " + REG;
                break;
            }
            case (int) MOV_TYPES_S::ADRESS:{
                QString REG;
                REG.append("(R").append(QString::number(OP%4)).append(")");
                COMMAND+=" " + QString::number(NUMBER) + ", " + REG;
                break;
            }
            }
            break;
        }
        case ROL:{
            COMMAND +="ROL";
            quint8 NUMBER = ((OP_CODE_BL & 0xF000)>>12) +1;

            quint8 OP = (OP_CODE_BL & 0x0F00)>>8;


            quint8 TYPE = getOPType(OP);

            switch (TYPE) {
            case (int) MOV_TYPES_S::REGISTER:{
                QString REG = QString::number(OP);
                COMMAND+=" " + QString::number(NUMBER) + ", " + REG;
                break;
            }
            case (int) MOV_TYPES_S::ADRESS:{
                QString REG;
                REG.append("(R").append(QString::number(OP%4)).append(")");
                COMMAND+=" " + QString::number(NUMBER) + ", " + REG;
                break;
            }
            }
            break;
        }
        case ROR:{
            COMMAND+="ROR";
            quint8 NUMBER = ((OP_CODE_BL & 0xF000)>>12) +1;

            quint8 OP = (OP_CODE_BL & 0x0F00)>>8;


            quint8 TYPE = getOPType(OP);

            switch (TYPE) {
            case (int) MOV_TYPES_S::REGISTER:{
                QString REG = QString::number(OP);
                COMMAND+=" " + QString::number(NUMBER) + ", " + REG;
                break;
            }
            case (int) MOV_TYPES_S::ADRESS:{
                QString REG;
                REG.append("(R").append(QString::number(OP%4)).append(")");
                COMMAND+=" " + QString::number(NUMBER) + ", " + REG;
                break;
            }
            }
            break;
        }
        case RCL:{
            COMMAND += "RCL";
            quint8 NUMBER = ((OP_CODE_BL & 0xF000)>>12) +1;

            quint8 OP = (OP_CODE_BL & 0x0F00)>>8;


            quint8 TYPE = getOPType(OP);

            switch (TYPE) {
            case (int) MOV_TYPES_S::REGISTER:{
                QString REG = QString::number(OP);
                COMMAND+=" " + QString::number(NUMBER) + ", " + REG;
                break;
            }
            case (int) MOV_TYPES_S::ADRESS:{
                QString REG;
                REG.append("(R").append(QString::number(OP%4)).append(")");
                COMMAND+=" " + QString::number(NUMBER) + ", " + REG;
                break;
            }
            }
            break;
        }
        case RCR:{
            COMMAND +="RCR";
            quint8 NUMBER = ((OP_CODE_BL & 0xF000)>>12) +1;

            quint8 OP = (OP_CODE_BL & 0x0F00)>>8;


            quint8 TYPE = getOPType(OP);

            switch (TYPE) {
            case (int) MOV_TYPES_S::REGISTER:{
                QString REG = QString::number(OP);
                COMMAND+=" " + QString::number(NUMBER) + ", " + REG;
                break;
            }
            case (int) MOV_TYPES_S::ADRESS:{
                QString REG;
                REG.append("(R").append(QString::number(OP%4)).append(")");
                COMMAND+=" " + QString::number(NUMBER) + ", " + REG;
                break;
            }
            }
            break;
        }

        case JMP:{
            COMMAND += "JMP";
            COMMAND += " " + QString::number(CPU->RAM[i+1],16).rightJustified(4,'0',true);
            IS_NUMBER = true;
            break;
        }

        case JZ:{
            COMMAND += "JZ";
            COMMAND += " " + QString::number(CPU->RAM[i+1],16).rightJustified(4,'0',true);
            IS_NUMBER = true;
            break;
        }

        case JNZ:{
            COMMAND +="JNZ";
            COMMAND += " " + QString::number(CPU->RAM[i+1],16).rightJustified(4,'0',true);
            IS_NUMBER = true;
            break;
        }
        case JGE:{
            COMMAND +="JGE";
            COMMAND += " " + QString::number(CPU->RAM[i+1],16).rightJustified(4,'0',true);
            IS_NUMBER = true;
            break;
        }
        case JG:{
            COMMAND +="JG";
            COMMAND += " " + QString::number(CPU->RAM[i+1],16).rightJustified(4,'0',true);
            IS_NUMBER = true;
            break;
        }
        case JLE:{
            COMMAND += "JLE";
            COMMAND += " " + QString::number(CPU->RAM[i+1],16).rightJustified(4,'0',true);
            IS_NUMBER = true;
            break;
        }
        case JL:{
            COMMAND +="JL";
            COMMAND += " " + QString::number(CPU->RAM[i+1],16).rightJustified(4,'0',true);
            IS_NUMBER = true;
            break;
        }
        case PIXEL:{
            COMMAND +="PIXEL";
            COMMAND += " " + QString::number(CPU->RAM[i+1] >> 8 | ((CPU->RAM[i+1]&0x00FF)<<8),16).rightJustified(4,'0',true)
                    + ", " + QString::number(CPU->RAM[i+2] >> 8 | ((CPU->RAM[i+2]&0x00FF)<<8),16).rightJustified(4,'0',true);
            IS_NUMBER = true;
            IS_NUMBER_PIXEL = true;
            break;
        }

        }

        }
        else if(IS_NUMBER_PIXEL == false) IS_NUMBER = false;
        else IS_NUMBER_PIXEL = false;


        LIST->addItem(
                        QString::number(i*2, 16).rightJustified(4,'0',true)
                       + " "
                      + QString::number(OP_CODE_LB,16).rightJustified(4,'0',true)
                      + "      "
                       + COMMAND.leftJustified(6, ' ', false)
                      );


    }
    int PC = CPU->PC;
    LIST->setCurrentRow(PC/2);
}

int Debugger::getOPType(unsigned char OP1, unsigned char OP2){
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

int Debugger::getOPType(unsigned char OP){
    if(OP>=0 && OP <= 3) return (quint8)MOV_TYPES_S::REGISTER;
    if(OP >= 4 && OP <= 7) return (quint8) MOV_TYPES_S::ADRESS;
    return (quint8) MOV_TYPES_S::UNDEFINED;
}
