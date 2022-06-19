#include "controlwidget.h"
#include "ui_controlwidget.h"
#include "processor.h"
#include "QErrorMessage"
#include <QDebug>
#include <QElapsedTimer>


Processor* CPU;
int SPEED = 1500;
ControlWidget::ControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlWidget)
{
    ui->setupUi(this);
    CPU = new Processor();
    connect(CPU, &Processor::dataChanged, this, &ControlWidget::onDataChanged);
    connect(CPU,&Processor::evokeException, this, &ControlWidget::onExceptionReceived);
    connect(this, &ControlWidget::doStep, CPU, &Processor::onStep);
    ui->velocitySlider->setTickPosition(QSlider::TicksAbove);
    ui->velocitySlider->setTickInterval(300);
    ui->velocitySlider->setMinimum(100);
    ui->velocitySlider->setMaximum(3000);
    ui->velocitySlider->setSingleStep(300);
    ui->velocitySlider->setPageStep(300);
    ui->velocitySlider->setFocusPolicy(Qt::FocusPolicy::NoFocus);

}




ControlWidget::~ControlWidget()
{

    delete ui;
}

void ControlWidget::on_startProcessor_clicked()
{
    this->on_compileProcessor_clicked();
    CPU->resetCommand();
    //CPU->reset();
//    unsigned short * PROGRAMM = new unsigned short[128];
//    for(int i = 0; i < 128; i++){
//        PROGRAMM[i] = 0x0000;
//    }



    CPU->isRunning = true;
   // delete[] PROGRAMM;
    while(CPU->isRunning){
        QElapsedTimer timer;
        timer.start();

        while ( timer.elapsed() < SPEED )
            QCoreApplication::processEvents();
        emit doStep();
        if(CPU->isRunning == false) break;

    }
//    CPU->loop();


}



void ControlWidget::onDataChanged(){
    ui->R0->setText(QString::number(CPU->REG[0],2).rightJustified(16, '0', true).insert(4,' ').insert(9,' ').insert(14,' ') + "  " +  QString::number(CPU->REG[0],16));
    ui->R1->setText(QString::number(CPU->REG[1],2).rightJustified(16, '0', true).insert(4,' ').insert(9,' ').insert(14,' ') + "  " +  QString::number(CPU->REG[1],16));
    ui->R2->setText(QString::number(CPU->REG[2],2).rightJustified(16, '0', true).insert(4,' ').insert(9,' ').insert(14,' ') + "  " +  QString::number(CPU->REG[2],10));
    ui->R3->setText(QString::number(CPU->REG[3],2).rightJustified(16, '0', true).insert(4,' ').insert(9,' ').insert(14,' ') + "  " +  QString::number(CPU->REG[3],10));


    ui->PC->setText(QString::number(CPU->PC,2).rightJustified(16,'0').insert(4,' ').insert(9,' ').insert(14,' ') + "  " +  QString::number(CPU->PC,10));

    QString ram = "";
    for(int i = 1; i <= 128; i++){
        ram+=QString::number(CPU->RAM[i-1],16).rightJustified(4,'0',true).insert(2,' ') + (" ");

    }
   // ui->RAM_VIEWER->setMaximumWidth(580);
    ui->RAM_VIEWER->setLineWrapMode(ui->RAM_VIEWER->FixedColumnWidth);
    ui->RAM_VIEWER->setLineWrapColumnOrWidth(24);
   // ui->RAM_VIEWER->setWordWrapMode(QtGui.QTextOption.WrapAtWordBoundaryOrAnywhere);
    ui->RAM_VIEWER->setText(ram);


}


void ControlWidget::onExceptionReceived(QString msg){
    qDebug()<<"ERROR";
    (new QErrorMessage(this))->showMessage(msg);
}

void ControlWidget::on_velocitySlider_valueChanged(int value)
{
    SPEED =  3100 - value;
    ui->SPEED->setText(QString::number(value));
}

void ControlWidget::on_compileProcessor_clicked()
{
    unsigned short * PROGRAMM = new unsigned short[128];
    for(int i = 0; i < 128; i++){
        PROGRAMM[i] = 0x0000;
    }

    PROGRAMM[0] = 0x4102;
    PROGRAMM[1] = 0xFFFF;


    CPU->writeRAM(PROGRAMM);

    delete[] PROGRAMM;
}

void ControlWidget::on_RESET_clicked()
{

    CPU->reset();
    CPU->isRunning = false;
}
