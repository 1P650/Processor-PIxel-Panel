#include "controlwidget.h"
#include "ui_controlwidget.h"
#include "processor.h"
#include "debugger.h"
#include "QErrorMessage"
#include <QDebug>
#include <QElapsedTimer>


Processor* CPU;
Debugger* DEG;
int SPEED = 1500;
ControlWidget::ControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlWidget)
{
    ui->setupUi(this);

    CPU = new Processor();
    DEG = new Debugger();
    connect(CPU, &Processor::dataChanged, this, &ControlWidget::onDataChanged);
    connect(CPU,&Processor::evokeException, this, &ControlWidget::onExceptionReceived);
    connect(this, &ControlWidget::doStep, CPU, &Processor::onStep);
    connect(CPU, &Processor::colorPixel, this, &ControlWidget::onColorPixel);
    ui->velocitySlider->setTickPosition(QSlider::TicksLeft);
    ui->velocitySlider->setTickInterval(300);
    ui->velocitySlider->setMinimum(300);
    ui->velocitySlider->setMaximum(3000);
    ui->velocitySlider->setSingleStep(300);
    ui->velocitySlider->setPageStep(300);
    ui->velocitySlider->setFocusPolicy(Qt::FocusPolicy::NoFocus);


    QGraphicsScene* PIXEL_SCENE = new QGraphicsScene(this);


    ui->PIXEL_PANEL->setScene(PIXEL_SCENE);
    ui->PIXEL_PANEL->setSceneRect(0,0,257,257);



    QPen PIXEL_PEN(Qt::black);

    QBrush PIXEL_BRUSH(Qt::black);


    quint8 PIXEL_SIZE = 32;

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if( (i%2 == 0 && j%2 == 0) || (i%2==1 && j%2 == 1)){
                PIXEL_PEN.setColor(Qt::gray);
                PIXEL_BRUSH.setColor(Qt::gray);
            }
            else{
                PIXEL_PEN.setColor(Qt::black);
                PIXEL_BRUSH.setColor(Qt::black);
            }
            PIXEL_SCENE->addRect(i*32,j*32,PIXEL_SIZE,PIXEL_SIZE, PIXEL_PEN, PIXEL_BRUSH);


        }


    }

    this->onDataChanged();

    //ui->PIXEL_PANEL->fitInView(PIXEL_SCENE->sceneRect(), Qt::KeepAspectRatio);





}




ControlWidget::~ControlWidget()
{

    delete ui;
}

void ControlWidget::on_startProcessor_clicked()
{
    //this->on_compileProcessor_clicked();
    //CPU->resetCommand();
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
    ui->R0->setText(  QString::number(CPU->REG[0],2).rightJustified(16, '0', true).insert(4,' ').insert(9,' ').insert(14,' ')
            + "    " +QString::number(CPU->REG[0],16).rightJustified(4, '0', true)
            + " " +  QString::number(CPU->REG[0],10).rightJustified(5,' ',true)
            + " " +  QString::number((qint16) CPU->REG[0]).rightJustified(5,' ',true)
            );

    ui->R1->setText(  QString::number(CPU->REG[1],2).rightJustified(16, '0', true).insert(4,' ').insert(9,' ').insert(14,' ')
            + "    " +QString::number(CPU->REG[1],16).rightJustified(4, '0', true)
            + " " +  QString::number(CPU->REG[1],10).rightJustified(5,' ',true)
            + " " +  QString::number((qint16)CPU->REG[1]).rightJustified(5,' ',true)
            );

    ui->R2->setText(  QString::number(CPU->REG[2],2).rightJustified(16, '0', true).insert(4,' ').insert(9,' ').insert(14,' ')
            + "    " +QString::number(CPU->REG[2],16).rightJustified(4, '0', true)
            + " " +  QString::number(CPU->REG[2],10).rightJustified(5,' ',true)
            + " " +  QString::number((qint16)CPU->REG[2]).rightJustified(5,' ',true)
            );

    ui->R3->setText(  QString::number(CPU->REG[3],2).rightJustified(16, '0', true).insert(4,' ').insert(9,' ').insert(14,' ')
            + "    " +QString::number(CPU->REG[3],16).rightJustified(4, '0', true)
            + " " +  QString::number(CPU->REG[3],10).rightJustified(5,' ',true)
            + " " +  QString::number((qint16)CPU->REG[3]).rightJustified(5,' ',true)
            );


    ui->PC->setText(  QString::number(CPU->PC,2).rightJustified(16, '0', true).insert(4,' ').insert(9,' ').insert(14,' ')
            + "    " +QString::number(CPU->PC,16).rightJustified(4, '0', true)
            + " " +  QString::number(CPU->PC,10).rightJustified(5,' ',true)
            + " " +  QString::number((qint16)CPU->PC).rightJustified(5,' ',true)
            );

    ui->SP->setText(  QString::number(CPU->SP,2).rightJustified(16, '0', true).insert(4,' ').insert(9,' ').insert(14,' ')
            + "    " +QString::number(CPU->SP,16).rightJustified(4, '0', true)
            + " " +  QString::number(CPU->SP,10).rightJustified(5,' ',true)
            + " " +  QString::number((qint16)CPU->SP).rightJustified(5,' ',true)
            );

    ui->PS->setText(  QString::number(CPU->PS,2).rightJustified(16, '0', true).insert(4,' ').insert(9,' ').insert(14,' ')
            + "    " +QString::number(CPU->PS,16).rightJustified(4, '0', true)
            + " " +  QString::number(CPU->PS,10).rightJustified(5,' ',true)
            + " " +  QString::number((qint16)CPU->PS).rightJustified(5,' ',true)
            );


    QString ram = "";
    QString RED = "<b><font color=\"Red\" weight=600>";
    QString endHtml = "</font></b>";

    for(int i = 1; i <= 128; i++){
        if((i-1)%4 == 0) {
            ram+= RED+(QString::number((i-1)*2,16).rightJustified(4,'0',true) + "   ")+endHtml;

        }
        ram+=QString::number(CPU->RAM[i-1],16).rightJustified(4,'0',true).insert(2,' ') + (" ");



    }
   // ui->RAM_VIEWER->setMaximumWidth(580);

   // ui->RAM_VIEWER->setWordWrapMode(QtGui.QTextOption.WrapAtWordBoundaryOrAnywhere);

    ui->RAM_VIEWER->setLineWrapMode(ui->RAM_VIEWER->FixedColumnWidth);
    ui->RAM_VIEWER->setLineWrapColumnOrWidth(38);
   // ui->RAM_VIEWER->setText(ram);
    ui->RAM_VIEWER->setHtml(ram);


    DEG->update(CPU, ui->DEBUGGER);



}


void ControlWidget::onExceptionReceived(QString msg){
    qDebug()<<"ERROR";
    (new QErrorMessage(this))->showMessage(msg);
}

void ControlWidget::on_velocitySlider_valueChanged(int value)
{
    int valueA = ((value+50)/300)*300;
    //ui->velocitySlider->setValue(valueA);
    SPEED =  3050 - valueA;
    ui->SPEED->setText(QString::number(valueA));
}

void ControlWidget::on_compileProcessor_clicked()
{
    unsigned short * PROGRAMM = new unsigned short[128];
    for(int i = 0; i < 128; i++){
        PROGRAMM[i] = 0x0000;

    }







  PROGRAMM[0] = 0x009C;
  PROGRAMM[1] = 0x0000;
  PROGRAMM[2] = 0x3536;

  PROGRAMM[3] = 0xD402;
  PROGRAMM[4] = 0x0100;

  PROGRAMM[5] = 0xD602;
  PROGRAMM[6] = 0x0800;

  PROGRAMM[7] = 0xD103;
  PROGRAMM[8] = 0x0100;

  PROGRAMM[9] = 0x004C;
  PROGRAMM[10] = 0x0000;

  PROGRAMM[11] = 0xD401;
  PROGRAMM[12]= 0x0000;

  PROGRAMM[13]= 0xD302;
  PROGRAMM[14]= 0x0001;

  PROGRAMM[15]= 0xD602;
  PROGRAMM[16]= 0x1400;

  PROGRAMM[17]= 0x3402;

  PROGRAMM[18]= 0xD101;
  PROGRAMM[19]= 0x0800;

  PROGRAMM[20]= 0xD304;

  PROGRAMM[21]= 0x0008;
  PROGRAMM[22]= 0x004C;

  PROGRAMM[23]= 0x0000;
  PROGRAMM[24]= 0xFFFF;




    CPU->writeRAM(PROGRAMM);

    delete[] PROGRAMM;
}

void ControlWidget::on_RESET_clicked()
{

    CPU->reset();
    CPU->isRunning = false;
    ui->PIXEL_PANEL->scene()->clear();

    QPen PIXEL_PEN(Qt::black);

    QBrush PIXEL_BRUSH(Qt::black);


    quint8 PIXEL_SIZE = 32;

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if( (i%2 == 0 && j%2 == 0) || (i%2==1 && j%2 == 1)){
                PIXEL_PEN.setColor(Qt::gray);
                PIXEL_BRUSH.setColor(Qt::gray);
            }
            else{
                PIXEL_PEN.setColor(Qt::black);
                PIXEL_BRUSH.setColor(Qt::black);
            }
            ui->PIXEL_PANEL->scene()->addRect(i*32,j*32,PIXEL_SIZE,PIXEL_SIZE, PIXEL_PEN, PIXEL_BRUSH);


        }
    }



}

void ControlWidget::onColorPixel(quint16 pos, quint16 color){
    quint8 X = (pos & 0xFF00)>>8;
    quint8 Y = pos &0x00FF;
    qDebug()<<"X: "<<X*32<<" Y: "<<Y*32;


    QPen T_PEN(QColor((int)color));
    QBrush T_BRUSH(QColor((int)color));
    ui->PIXEL_PANEL->scene()->addRect(X*32,Y*32,32,32, T_PEN, T_BRUSH);
}

void ControlWidget::on_STOP_clicked()
{

    CPU->isRunning = false;
}

void ControlWidget::on_STEP_clicked()
{
    CPU->isRunning = false;
    CPU->isRunning = true;
    emit doStep();
    CPU->isRunning = false;
}
