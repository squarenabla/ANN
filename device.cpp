#include "device.h"

Device::Device()
{
    devh = NULL;
}

Device::~Device(){
//    libusb_release_interface(devh, 0);
//    libusb_exit(NULL);
}

int Device::Init(){
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

int Device::Interrupt(QVector<quint32> &data){
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

    for(int i = 0; i < DATASIZE; i++) {
      //  std::cout<< (char)pData[i]<<"_";
        //pData[i] = (unsigned char)i;
        //qDebug()<<(unsigned char)pData[i];
        data[i] = i;
    }
    //std::copy(pData, pData+size, data);
    //resultSize = size;

    //qDebug()<< actualBytesTransfered;
    return 0;
}

