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
    void PlotWaveletTransformation(const WaveletTransform &yData);

    void on_spinBox_valueChanged(int arg1);

    void on_learnButton_clicked();

    void changeInstruction(const Movement movement);


    void on_upBtn_clicked();

    void on_restBtn_clicked();

    void on_downBtn_clicked();

    void on_leftBtn_clicked();

    void on_rightBtn_clicked();

    void on_stopBtn_clicked();

    void on_executeButton_clicked();

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
