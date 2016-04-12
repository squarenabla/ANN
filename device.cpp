#include "device.h"

Device::Device(QObject *parent) : QThread(parent) {
    devh = NULL;
    timeKey = 0;
    buffer.fill(QVector<qreal>(BUFFER_SIZE, 0.0), ELECTRODENUM);
    lastData.fill(0, ELECTRODENUM);
    file.open("./data/data1.txt");
}

Device::~Device() {
//    libusb_release_interface(devh, 0);
//    libusb_exit(NULL);
    file.close();
}

void Device::run() {
    quint32 seed = 0;
    QVector<double> fourierTransform(BUFFER_SIZE);
    while(!file.eof()) {
        mutex.lock();
        for (int i = 0; i < ELECTRODENUM; i++) {
            qreal value;
            file >> value;
           // if (seed % 33 == 0) {
                lastData[i] = (quint32) qAbs(value * 1000);
                buffer[i].push_back(value);
                buffer[i].pop_front();
                //lastData[i] = qrand() % 10;
           // }
        }
        //if (seed % 33 == 0) {
            FFTAnalysis(buffer[6], fourierTransform, BUFFER_SIZE, BUFFER_SIZE);
           // qDebug() << buffer[6];
           // qDebug() << fourierTransform;
            //if (seed % 33 == 0){
              //  buffer.push_back(lastData);
              //  buffer.pop_front();
            emit FourierTranformation(fourierTransform);
            emit DataReceived(timeKey, lastData);
        //}
        timeKey++;
        seed++;
        mutex.unlock();
        msleep(5);
    }
    exec();
}

int Device::Init() {
    int r;

//    r = libusb_init(NULL);
//    if (r < 0) {
//        return r;
//    }

//    devh = libusb_open_device_with_vid_pid(NULL, VID, PID);

//    if (!devh) {
//        qDebug() << "Error finding USB device";
//        return -1;
//    }

//    libusb_set_auto_detach_kernel_driver(devh, 1);
//    //libusb_attach_kernel_driver(devh, 1);

//    r = libusb_claim_interface(devh, 0);

//    if (r !=0 ) {
//        qDebug() << "Error claiming interface";
//        return -2;
//    }
    return 0;
}

void Device::Interrupt(QVector<EMGFourier> &data, const Movement movement) {
    int r;
  //  qDebug()<<"Interrupt";
 //   int size = 32;
//    int actualBytesTransfered;
    //unsigned char pData[size];
//    r = libusb_interrupt_transfer(devh, ENDP, &pData[0], size, &actualBytesTransfered, 1000);

//    if (r != 0) {
//        qDebug() << "Error libusb_interrupt_transfer" ;
//        return -3;
//    }

 //   for (int i = 0; i < DATASIZE; i++) {
      //  std::cout<< (char)pData[i]<<"_";
        //pData[i] = (unsigned char)i;
        //qDebug()<<(unsigned char)pData[i];
 //       data[i] = i;
 //   }
    std::ifstream input;

    if (movement == REST) {
        input.open("./data/rest");
    }
    if (movement == UP) {
        input.open("./data/up");
    }

    quint32 seed = 0;
    quint32 key = 0;


    while (!input.eof() && input.is_open()) {
        for (int i = 0; i < ELECTRODENUM; i++) {
            qreal value;
            input >> value;
        //    if (seed % 16 == 0) {
                //lastData[i] = (quint32) qAbs(value * 1000);
                buffer[i].push_back(value);
                buffer[i].pop_front();
                //lastData[i] = qrand() % 10;
         //   }

            if (key > BUFFER_SIZE) {
                QVector<double> fourierTransform(BUFFER_SIZE);
                FFTAnalysis(buffer[i], fourierTransform, BUFFER_SIZE, BUFFER_SIZE);
                data[i].push_back(fourierTransform);
                //data[i].takeLast() = fourierTransform;
            }
        }
        qDebug() << key;
       // if(seed % 16 == 0)
        if (key > BUFFER_SIZE) {
            key = 0;
        }
        else {
            key++;
        }
        //seed++;
    }


}

int Device::GetBuffer(QVector<QVector<quint32> > &data) {
    mutex.lock();
    //data = buffer;
    mutex.unlock();
}
