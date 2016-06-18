#include "device.h"

Device::Device(QObject *parent) :
QThread(parent), devh(NULL), timeKey(0) {
//    devh = NULL;
//    timeKey = 0;
    buffer.fill(QVector<qreal>(BUFFER_SIZE, 0.0), ELECTRODE_NUM);
    lastData.fill(0, ELECTRODE_NUM);
    file.open("./data/data1.txt");
}

Device::~Device() {
//    libusb_release_interface(devh, 0);
//    libusb_exit(NULL);
    file.close();
    libusb_release_interface(devh, 0);
    libusb_exit(NULL);
}

void Device::run() {
    quint32 seed = 0;
    quint32 key = 0;
    QVector<EMGRMS> data(ELECTRODE_NUM);



    USB_Frame_t frame;
    int size = 64;
    int actualBytesTransfered;
    unsigned char pData[size];

    int t0 = time(0);
    unsigned char buf[64];

//    for(int n = 0; ; n++ ) {
//      r = libusb_interrupt_transfer(devh, 1 | LIBUSB_ENDPOINT_IN, (unsigned char*)&pData, size, &actualBytesTransfered, 1000);


//      if (r != 0) {
//        qDebug() << "Error libusb_interrupt_transfer " << r <<" "<< actualBytesTransfered;
//        exec();
//      } else {
//        //std::cout<< "Transmitted " << actualBytesTransfered << std::endl;
//      }

//      memcpy(&frame, &pData, sizeof(frame));


//      qDebug() << frame.i << " v1 = "<< frame.samples[0].v1 << " v2 = "<< frame.samples[0].v2 <<  " v3 = "<< frame.samples[0].v3 << " v4 = "<< frame.samples[0].v4;

//      int t1 = time(0);

//      if( (t1 - t0) > 0 ) {
//        qDebug() << (float)7.0 * n / (t1 - t0);
//      }
//    }


    int r;
    std::ofstream outFile("./data/deviceoutput.txt");

    while(1) {
    //    mutex.lock();
        r = libusb_interrupt_transfer(devh, 1 | LIBUSB_ENDPOINT_IN, (unsigned char*)&pData, size, &actualBytesTransfered, 1000);
        if (r != 0) {
            qDebug() << "Error libusb_interrupt_transfer " << r <<" "<< actualBytesTransfered;
            exec();
        } else {
        //std::cout<< "Transmitted " << actualBytesTransfered << std::endl;
        }

        memcpy(&frame, &pData, sizeof(frame));
        QVector<qreal> vecData(4);
        vecData[0] = (qreal)frame.samples[0].v1/MAX_UINT16;
        vecData[1] = (qreal)frame.samples[0].v2/MAX_UINT16;
        vecData[2] = (qreal)frame.samples[0].v3/MAX_UINT16;
        vecData[3] = (qreal)frame.samples[0].v4/MAX_UINT16;

        //qDebug() << vecData;

        outFile << std::endl;

        for (int i = 0; i < ELECTRODE_NUM; i++) {

            qreal value = vecData[i];
//            file >> value;
            outFile << value << " ";
            lastData[i] = value;
           // if (seed % 16 == 0) {

                buffer[i].push_back(value);
                buffer[i].pop_front();
                //lastData[i] = qrand() % 10;
          //  }
            if (key > BUFFER_SIZE) {
                data[i].clear();
                //FourierTransform transformArray(BUFFER_SIZE);
                //EMGFourierVec transformArray;

                //Wavelet transform --debug
                //WaveletHaar(buffer[i], transformArray);
                //WaveletRMS(transformArray, rmsArray);

                double rms = RootMeanSquare(buffer[i]);
                data[i].push_back(rms);

                if (i == 0) {
                    //Fourier transform
                    //FourierTransform transformArray(BUFFER_SIZE);
                    //FFTAnalysis(buffer[i], transformArray, BUFFER_SIZE, BUFFER_SIZE);
                    //emit FourierTranformation(transformArray);

                    WaveletTransform transformArray(BUFFER_SIZE);
                    WaveletHaar(buffer[i], transformArray);
                    emit WaveletTransformation(transformArray);

                    //qDebug() << transformArray;
                //    WaveletRMS(transformArray, rmsArrar);
                //    qDebug() << rmsArrar;
                }


            }
        }

     //   if (seed % 16 == 0) {
            if (key > BUFFER_SIZE) {
             //   key = 0;
                 //emit FourierTranformation(data);
                 emit RMSTransformation(data, timeKey);
            }
            else {
                key++;
            }
    //    }
        //if (seed % 33 == 0) {

           // qDebug() << buffer[6];
           // qDebug() << fourierTransform;
            //if (seed % 33 == 0){
              //  buffer.push_back(lastData);
              //  buffer.pop_front();

            emit DataReceived(timeKey, lastData);
        //}
        timeKey++;
        seed++;
   //     mutex.unlock();
        msleep(10);
    }
    exec();
}

int Device::Init() {
    int r = libusb_init(NULL);
    if (r < 0) {
        qDebug() << r;
        return r;
    }

    devh = NULL;
    devh = libusb_open_device_with_vid_pid(NULL, VID, PID);

    if (!devh) {
        qDebug() << "Error finding USB device";
        return -1;
    }

    libusb_set_auto_detach_kernel_driver(devh, 1);

    r =  libusb_claim_interface(devh, 0);
    if (r != 0) {
        qDebug() << "Error claiming interface";
        return -2;
    }

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

void Device::WaveletInterrupt(QVector<EMGWavelet> &data, const Movement movement) {
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

 //   for (int i = 0; i < DATA_SIZE; i++) {
      //  std::cout<< (char)pData[i]<<"_";
        //pData[i] = (unsigned char)i;
        //qDebug()<<(unsigned char)pData[i];
 //       data[i] = i;
 //   }
    std::ifstream input;
    std::ofstream output;

    if (movement == REST) {
        input.open("./data/devrest");
    //    output.open("./data/results/rest");
    }
    if (movement == UP) {
        input.open("./data/up");
    //    output.open("./data/results/up");
    }
    if (movement == DOWN) {
        input.open("./data/down");
    //    output.open("./data/results/down");
    }
    if (movement == LEFT) {
        input.open("./data/left");
    //    output.open("./data/results/devleft");
    }
    if (movement == RIGHT) {
        input.open("./data/right");
    //    output.open("./data/results/devright");
    }

    quint32 seed = 0;
    quint32 key = 0;


    while (!input.eof() && input.is_open()) {
        for (int i = 0; i < ELECTRODE_NUM; i++) {
            qreal value;
            input >> value;
          //  if (seed % 16 == 0) {
                //lastData[i] = (quint32) qAbs(value * 1000);
                buffer[i].push_back(value);
                buffer[i].pop_front();
                //lastData[i] = qrand() % 10;
           // }



            if (key > BUFFER_SIZE) {
                QVector<double> transformArray(BUFFER_SIZE);
                QVector<double> rmsArray;
                QVector<double> highFreqRms;
                //Fourier transform

                //FFTAnalysis(buffer[i], transformArray, BUFFER_SIZE, BUFFER_SIZE);
                //data[i].push_back(transformArray);
                //Wavelet transform --debug
                WaveletHaar(buffer[i], transformArray);
               // if (i == 0) {
               //     emit WaveletTransformation(transformArray);
               // }
                    //qDebug() << transformArray;
                WaveletRMS(transformArray, rmsArray);
                //qDebug() << i << rmsArray;
                highFreqRms.push_back(rmsArray[rmsArray.size() - 1]);
                highFreqRms.push_back(rmsArray[rmsArray.size() - 2]);

                data[i].push_back(highFreqRms);
                //qDebug() << rmsArray;
                //output << rmsArray[rmsArray.size() - 2]<<" ";
            }
        }
        //qDebug() << key;
        //if(seed % 16 == 0) {
            if (key > BUFFER_SIZE) {
                //output << "\n";
                key = key;
            }
            else {
                key++;
            }
        //}
        seed++;

    }



//    qDebug() << "deviceDATA_SIZE:" << data.size();
//    //qDebug() << data[0].size();
//    for(int i=0; i<data.size();i++){
//        qDebug() << data[i].size();
//    }

    input.close();
    //output.close();
}

void Device::RMSInterrupt(QVector<EMGRMS> &data, const Movement movement) {
    std::ifstream input;
    std::ofstream output;

    if (movement == REST) {
        input.open("./data/rest.txt");
    //    output.open("./data/results/devrest");
    }
    if (movement == UP) {
        input.open("./data/up.txt");
    //    output.open("./data/results/up");
    }
    if (movement == DOWN) {
        input.open("./data/down.txt");
    //    output.open("./data/results/down");
    }
    if (movement == LEFT) {
        input.open("./data/left.txt");
    //    output.open("./data/results/devleft");
    }
    if (movement == RIGHT) {
        input.open("./data/right.txt");
    //    output.open("./data/results/devright");
    }

    quint32 seed = 0;
    quint32 key = 0;


    while (!input.eof() && input.is_open()) {
        for (int i = 0; i < ELECTRODE_NUM; i++) {
            qreal value;
            input >> value;
          //  if (seed % 16 == 0) {
                //lastData[i] = (quint32) qAbs(value * 1000);
                buffer[i].push_back(value);
                buffer[i].pop_front();
                //lastData[i] = qrand() % 10;
           // }



            if (key > BUFFER_SIZE) {
                double rms = RootMeanSquare(buffer[i]);;

                data[i].push_back(rms);
                //qDebug() << rmsArray;
                //output << rmsArray[rmsArray.size() - 2]<<" ";
            }
        }
        //qDebug() << key;
        //if(seed % 16 == 0) {
            if (key > BUFFER_SIZE) {
                //output << "\n";
                key = key;
            }
            else {
                key++;
            }
        //}
        seed++;

    }



    input.close();
    //output.close();
}


void Device::Interrupt(QVector<EMGFourier> &data, const Movement movement) {
    std::ifstream input;

    if (movement == REST) {
        input.open("./data/rest");
    }
    if (movement == UP) {
        input.open("./data/up");
    }
    if (movement == DOWN) {
        input.open("./data/down");
    }
    if (movement == LEFT) {
        input.open("./data/left");
    }
    if (movement == RIGHT) {
        input.open("./data/right");
    }

    quint32 seed = 0;
    quint32 key = 0;

    while (!input.eof() && input.is_open()) {
        for (int i = 0; i < ELECTRODE_NUM; i++) {
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
        //qDebug() << key;
       // if(seed % 16 == 0)
        if (key > BUFFER_SIZE) {
            key = 0;
        }
        else {
            key++;
        }
        //seed++;
    }

    input.close();
}

int Device::GetBuffer(QVector<QVector<quint32> > &data) {
    mutex.lock();
    //data = buffer;
    mutex.unlock();
}
