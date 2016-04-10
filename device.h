#ifndef DEVICE_H
#define DEVICE_H

#define VID 0x0483
#define PID 0x5750
#define ENDP 0x81

#define BUFFER_SIZE 50

#include <QThread>
#include <QMutex>
#include <QDebug>
#include <QVector>
#include <algorithm>

#include <libusb-1.0/libusb.h>

#include "types.h"


class Device : public QThread {
    Q_OBJECT
public:
    explicit Device(QObject *parent = 0);
    ~Device();

    int Init();

    int Interrupt(QVector<quint32> &data);

    void run();

    int GetBuffer(QVector<QVector<quint32> > &data);

private:
    struct libusb_device_handle *devh;
    QVector<quint32> lastData;
    QVector<QVector<quint32> > buffer;
    QMutex mutex;
    quint32 timeKey;
signals:
    void DataReceived(quint32 key, ElectrodeEMG data);
};

#endif // DEVICE_H
