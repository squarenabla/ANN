#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>

//#include "device.h"
#include "controler.h"
#include "error.h"

namespace Ui {
    class View;
}

class View : public QMainWindow{
    Q_OBJECT

public:
    explicit View(QWidget *parent = 0);
    ~View();

private slots:
//    void on_pushButton_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_learnButton_clicked();

signals:
    void startLearning();

private:
    void _ShowError(QString errString);
    bool _ValidateTable();

    Ui::View *ui;
    //Device *usbDevice;
    Controler _controler;
    Delegate *_tableDelegate;
};

#endif // VIEW_H
