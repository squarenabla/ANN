#ifndef DEVICE_H
#define DEVICE_H

#define VID 0x0483
#define PID 0x5750
#define ENDP 0x81

#include <QDebug>
#include <QVector>
#include <algorithm>

#include <libusb-1.0/libusb.h>

#include "types.h"


class Device{
public:
    Device();
    ~Device();

    int Init();

    int Interrupt(QVector<quint32> &data);

private:
    struct libusb_device_handle *devh;
};

#endif // DEVICE_H
