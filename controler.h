#ifndef CONTROLER_H
#define CONTROLER_H



#include <QObject>
#include <QThread>

#include "types.h"
#include "teacher.h"
#include "executor.h"
#include "device.h"



class Controler : public QObject {
    Q_OBJECT
public:
    explicit Controler(QObject *parent = 0);
    ~Controler();

public slots:

    int SetTeacher();
    int SetANNLayers(const ANNLayers hiddenLayers);
    void Teach();
    void Execute();

    void ShowResults();

signals:
    void MovementChanged(const Movement direction);
    void DataReceived(quint32 key, ElectrodeEMG data);
    void FourierTranformation(FourierTransform data);
private:
    int _Move(const Movement direction);
    void _TransferData(const QVector<quint32> data, const Movement direction);


    Teacher _teacher;
    Executor _executor;
    Device  _usbDevice;
    QVector<EMGFourierData> _EMGresults;
};

#endif // CONTROLER_H
