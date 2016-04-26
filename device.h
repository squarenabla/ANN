#ifndef DEVICE_H
#define DEVICE_H

#define VID 0x0483
#define PID 0x5750
#define ENDP 0x81

#include <QThread>
#include <QMutex>
#include <QDebug>
#include <QVector>
#include <algorithm>
#include <fstream>

#include <libusb-1.0/libusb.h>

#include "types.h"
#include "Algerba/fft.h"
#include "Algerba/wavelet.h"
#include "Algerba/rms.h"

class Device : public QThread {
    Q_OBJECT
public:
    explicit Device(QObject *parent = 0);
    ~Device();

    int Init();

    void Interrupt(QVector<EMGFourier> &data, const Movement movement);
    void WaveletInterrupt(QVector<EMGWavelet> &data, const Movement movement);
    void RMSInterrupt(QVector<EMGRMS> &data, const Movement movement);

    void run();

    int GetBuffer(QVector<QVector<quint32> > &data);

private:
    struct libusb_device_handle *devh;
    QVector<qreal> lastData;
    QVector<QVector<qreal> > buffer;


    QMutex mutex;
    quint32 timeKey;
    std::ifstream file;

signals:
    void DataReceived(quint32 key, ElectrodeEMG data);
    void FourierTranformation(EMGFourierVec data);
    void WaveletTransformation(const EMGWaveletVec &data);
    void RMSTransformation(const EMGRMSVec data, const quint32 time);
};

#endif // DEVICE_H
