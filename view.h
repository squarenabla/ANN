#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QThread>

#include "qcustomplot.h"
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
    void setupPlot(const quint16 graphNum);
    void plotEMGData(const quint32 key, const ElectrodeEMG &data);
    void PlotFourierTransformation(FourierTransform yData);

    void on_spinBox_valueChanged(int arg1);

    void on_learnButton_clicked();

    void changeInstruction(const Movement movement);

signals:
    void startLearning();

private:
    void _ShowError(QString errString);
    bool _ValidateTable();

    Ui::View *ui;
    //Device *usbDevice;
    Controler *_controler;
    Delegate *_tableDelegate;
    QThread workerThread;
};

#endif // VIEW_H
