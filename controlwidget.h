#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>


namespace Ui {
class ControlWidget;
}

class ControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlWidget(QWidget *parent = 0);
    ~ControlWidget();
public slots:
    void onDataChanged();
    void onExceptionReceived(QString msg);
private slots:
    void on_startProcessor_clicked();
    void on_velocitySlider_valueChanged(int value);

    void on_compileProcessor_clicked();

    void on_RESET_clicked();

    void onColorPixel(quint16 pos, quint16 color);

    void on_STOP_clicked();

    void on_STEP_clicked();

signals:
    void doStep();





private:
    Ui::ControlWidget *ui;
};

#endif // CONTROLWIDGET_H
