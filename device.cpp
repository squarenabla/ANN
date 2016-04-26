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
}

void Device::run() {
    quint32 seed = 0;
    quint32 key = 0;
    QVector<EMGRMS> data(ELECTRODE_NUM);

    while(!file.eof()) {
    //    mutex.lock();
        for (int i = 0; i < ELECTRODE_NUM; i++) {
            qreal value;
            file >> value;
            lastData[i] = value;
           // if (seed % 16 == 0) {

                buffer[i].push_back(value);
                buffer[i].pop_front();
                //lastData[i] = qrand() % 10;
          //  }
            if (key > BUFFER_SIZE) {
                data[i].clear();
                //Fourier transform
                //FFTAnalysis(buffer[i], transformArray, BUFFER_SIZE, BUFFER_SIZE);
                //Wavelet transform --debug
                //WaveletHaar(buffer[i], transformArray);
                //WaveletRMS(transformArray, rmsArray);

                double rms = RootMeanSquare(buffer[i]);
                data[i].push_back(rms);

                //if (i == 0) {
                    //emit WaveletTransformation(transformArray);
                    //qDebug() << transformArray;
                //    WaveletRMS(transformArray, rmsArrar);
                //    qDebug() << rmsArrar;
                //}


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
        input.open("./data/rest");
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
    //    output.open("./data/results/left");
    }
    if (movement == RIGHT) {
        input.open("./data/right");
    //    output.open("./data/results/right");
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
        input.open("./data/rest");
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
    //    output.open("./data/results/left");
    }
    if (movement == RIGHT) {
        input.open("./data/right");
    //    output.open("./data/results/right");
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
