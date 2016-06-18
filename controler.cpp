#include "controler.h"



Controler::Controler(QObject *parent) : QObject(parent) {
    _usbDevice.Init();
    //_usbDevice.start();

    _executor.SetANN(FANN_FILE_NAME);

    connect(&_usbDevice, SIGNAL(DataReceived(quint32,ElectrodeEMG)), this, SIGNAL(DataReceived(quint32,ElectrodeEMG)));
    connect(&_usbDevice, SIGNAL(FourierTranformation(FourierTransform)), this, SIGNAL(FourierTranformation(FourierTransform)));
    //connect(&_usbDevice, SIGNAL(), this, SIGNAL(FourierTranformation(FourierTransform)));
    //connect(&_usbDevice, SIGNAL(FourierTranformation(EMGFourierVec)), this, SLOT(RealTimeExecute(EMGFourierVec)));
    connect(&_usbDevice, SIGNAL(WaveletTransformation(WaveletTransform)), this, SIGNAL(WaveletTransformation(WaveletTransform)));
    //connect(&_usbDevice, SIGNAL(WaveletTransformation(EMGWaveletVec)), this, SLOT(RealTimeExecute(EMGWaveletVec)));
    connect(&_usbDevice, SIGNAL(RMSTransformation(EMGRMSVec, quint32)), this, SLOT(RealTimeExecute(EMGRMSVec, quint32)));
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
    for (int i = 0; i < TRAIN_NUM; i++) {
        emit LearnMovementChanged(trainSequence[i]);
        //QThread::sleep(2);
        _Move(trainSequence[i]);
    }
    //ShowResults();

    _teacher.createTrainData(_EMGsequence);
    _teacher.trainOnData();

    _executor.SetANN(FANN_FILE_NAME);

    qDebug() <<"size"<< _EMGresults.size();
   // for (int i = 0; i < _EMGresults.size(); i++) {
   //     emit FourierTranformation(_EMGresults[i].waveletArray[6]);
      //  qDebug() << _EMGresults[i].movementIndex;
   //     QThread::msleep(20);
   // }
}

void Controler::Execute(const Movement direction) {
    //Movement direction = DOWN;
    QVector<EMGRMS> data(ELECTRODE_NUM);
    QVector<float> result;

    _usbDevice.RMSInterrupt(data, direction);

    EMGRMSData EMGRMSElement;
    EMGRMSElement.movementIndex = direction;
    EMGRMSElement.rmsArray.resize(ELECTRODE_NUM);
    //qDebug() << data;
    qDebug() << direction << ":";
    for (int i = 0; i < data[0].size(); i++) {
        for (int j = 0; j < ELECTRODE_NUM; j++) {
            EMGRMSElement.rmsArray[j] = data[j][i];
        }
        //_EMGresults.push_back(EMGFourierElement);
        _executor.Execute(EMGRMSElement, result);
        qDebug() << result;
    }
}

void Controler::StartRTExecution() {
    _usbDevice.start();
}

void Controler::StopRTExecution() {
    _usbDevice.terminate();
}

void Controler::RealTimeExecute(const EMGRMSVec data, const quint32 time) {
    EMGRMSData EMGRMSElement;
    QVector<float> result;

    EMGRMSElement.movementIndex = REST;
    EMGRMSElement.rmsArray.resize(ELECTRODE_NUM);

    for (int i = 0; i < data[0].size(); i++) {
        for (int j = 0; j < ELECTRODE_NUM; j++) {
            EMGRMSElement.rmsArray[j] = data[j][i];
        }
        //_EMGresults.push_back(EMGFourierElement);
//Ececturion
        _executor.Execute(EMGRMSElement, result);

        qDebug() << MaxElement(result.begin(), result.end());
        Movement movement = (Movement)MaxElement(result.begin(), result.end());
        emit ExecuteMovementChanged(movement);


      //  qDebug()
//        if (_result.size() < BUFFER_SIZE) {
 //           _result.push_back(result[0]);
//        }
//        else {
//            qSort(_result.begin(), _result.end());
//            qDebug() << _result[_result.size()/2];
//            _result.clear();
//        }

    }

  //  qDebug() << time << result;
}

int Controler::_Move(const Movement direction) {
 //   for (unsigned int i = 0; i < INTERRUPT_NUM; ++i) {
        QVector<EMGRMS> data(ELECTRODE_NUM);
        _usbDevice.RMSInterrupt(data, direction);
        //_TransferData(data, direction);
  //  }
    EMGcharacteristics emgCharacteristic;
    emgCharacteristic.movementIndex = direction;


    EMGRMSData EMGRMSElement;
    EMGRMSElement.movementIndex = direction;
    EMGRMSElement.rmsArray.resize(ELECTRODE_NUM);

//    EMGFourierData EMGFourierElement;
//    EMGFourierElement.movementIndex = direction;
//    EMGFourierElement.fourierArray.resize(ELECTRODE_NUM);
//    qDebug() << "DATA_SIZE:" << data.size();
//    //qDebug() << data[0].size();
//    for(int i=0; i<data.size();i++){
//        qDebug() << data[i].size();
//    }

//    qDebug() << "---------------------";

    for (int i = 0; i < data[0].size(); i++) {
        for (int j = 0; j < ELECTRODE_NUM; j++) {
            emgCharacteristic.rms[j] = data[j][i];
        }
        _EMGsequence.push_back(emgCharacteristic);
    }
//    qDebug() << "resultsize:" << _EMGresults.size();
    return 0;
}

void Controler::_TransferData(const QVector<quint32> data, const Movement direction) {
    qDebug()<<"_TransferData";
}

void Controler::ShowResults(){
}
