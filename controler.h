#ifndef CONTROLER_H
#define CONTROLER_H



#include <QObject>
#include <QThread>

#include "types.h"
#include "Algorithm/findmax.h"
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
    void Execute(const Movement direction);

    void StartRTExecution();
    void StopRTExecution();
    void RealTimeExecute(const EMGRMSVec data, const quint32 time);
    void ShowResults();

signals:
    void LearnMovementChanged(const Movement direction);
    void ExecuteMovementChanged(const Movement direction);
    void DataReceived(quint32 key, ElectrodeEMG data);
   // void FourierTranformation(FourierTransform data);
    void WaveletTransformation(const WaveletTransform &data);
    void FourierTranformation(FourierTransform data);
private:
    int _Move(const Movement direction);
    void _TransferData(const QVector<quint32> data, const Movement direction);


    Teacher                     _teacher;
    Executor                    _executor;
    Device                      _usbDevice;
    QVector<EMGcharacteristics> _EMGsequence;
    QVector<EMGRMSData>         _EMGresults;
    QVector<float>              _result;
};

#endif // CONTROLER_H
