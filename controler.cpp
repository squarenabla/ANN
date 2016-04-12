#include "controler.h"



Controler::Controler(QObject *parent) : QObject(parent) {
    _usbDevice.Init();
    //_usbDevice.start();
    _executor.SetANN(FANNFILENAME);
    connect(&_usbDevice, SIGNAL(DataReceived(quint32,ElectrodeEMG)), this, SIGNAL(DataReceived(quint32,ElectrodeEMG)));
    connect(&_usbDevice, SIGNAL(FourierTranformation(FourierTransform)), this, SIGNAL(FourierTranformation(FourierTransform)));
}

Controler::~Controler() {
    _usbDevice.terminate();
    _usbDevice.wait();
}

int Controler::SetTeacher() {
    return 0;
}

int Controler::SetANNLayers(const ANNLayers hiddenLayers) {
    _teacher.setLayers(hiddenLayers);
    return 0;
}

void Controler::Teach() {
    for (int i = 0; i < TRAINNUM; i++) {
        emit MovementChanged(trainSequence[i]);
        //QThread::sleep(2);
        _Move(trainSequence[i]);
    }
    //ShowResults();
    _teacher.createTrainData(_EMGresults);
    _teacher.trainOnData();

    qDebug() << _EMGresults.size();
    for (int i = 0; i < _EMGresults.size(); i++) {
        emit FourierTranformation(_EMGresults[i].fourierArray[6]);
      //  qDebug() << _EMGresults[i].movementIndex;
        QThread::msleep(20);
    }
}

void Controler::Execute() {
    Movement direction = REST;
    QVector<EMGFourier> data(ELECTRODENUM);
    QVector<float> result;

    _usbDevice.Interrupt(data, direction);

    EMGFourierData EMGFourierElement;
    EMGFourierElement.movementIndex = direction;
    EMGFourierElement.fourierArray.resize(ELECTRODENUM);
    //qDebug() << data;
    for (int i = 0; i < data[0].size(); i++) {
        for (int j = 0; j < ELECTRODENUM; j++) {
            EMGFourierElement.fourierArray[j] = data[j][i];
        }
        //_EMGresults.push_back(EMGFourierElement);
        _executor.Execute(EMGFourierElement, result);
        qDebug() << result;
    }
}

int Controler::_Move(const Movement direction) {
 //   for (unsigned int i = 0; i < INTERRUPTNUM; ++i) {
        QVector<EMGFourier> data(ELECTRODENUM);
        _usbDevice.Interrupt(data, direction);
        //_TransferData(data, direction);
  //  }
    EMGFourierData EMGFourierElement;
    EMGFourierElement.movementIndex = direction;
    EMGFourierElement.fourierArray.resize(ELECTRODENUM);
    //qDebug() << data;
    for (int i = 0; i < data[0].size(); i++) {
        for (int j = 0; j < ELECTRODENUM; j++) {
            EMGFourierElement.fourierArray[j] = data[j][i];
        }
        _EMGresults.push_back(EMGFourierElement);
    }
    return 0;
}

void Controler::_TransferData(const QVector<quint32> data, const Movement direction) {
    qDebug()<<"_TransferData";
}

void Controler::ShowResults(){
}
