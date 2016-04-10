#include "controler.h"



Controler::Controler(QObject *parent) : QObject(parent) {
    _usbDevice.Init();
    _usbDevice.start();
    connect(&_usbDevice, SIGNAL(DataReceived(quint32,ElectrodeEMG)), this, SIGNAL(DataReceived(quint32,ElectrodeEMG)));
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
        QThread::sleep(2);
        _Move(trainSequence[i]);
    }
    ShowResults();
    _teacher.createTrainData(_EMGresults);
    _teacher.trainOnData();
}

void Controler::Execute() {
    QVector<quint32> data(DATASIZE);
    QVector<float> rdata(DATASIZE);
    QVector<float> result;

    _executor.setANN(FANNFILENAME);

    _usbDevice.Interrupt(data);

    std::transform(data.begin(), data.end(), rdata.begin(), [](quint32 val) -> float {
                   return (float)val/MAXAMLVALUE;
    });

    _executor.execute(rdata, result);
    printVector(result);
}

int Controler::_Move(const Movement direction) {
//    unsigned char resultsArray[INTERRUPTNUM*DATASIZE];
//    for(unsigned int i=0; i<INTERRUPTNUM; i++){
//        //int resultSize = 0;
//        //unsigned char result[DATASIZE];
//        _usbDevice.Interrupt(resultsArray + DATASIZE*i);
//    }

//    qDebug()<<"---------------------";
//    for(int i=0; i<INTERRUPTNUM*DATASIZE; ++i){
//        qDebug()<<resultsArray[i];
//    }
//    qDebug()<<"---------------------";

//    _TransferData(resultsArray, direction);

    for (unsigned int i = 0; i < INTERRUPTNUM; ++i) {
        QVector<quint32> data(DATASIZE);
        _usbDevice.Interrupt(data);
        _TransferData(data, direction);
    }
    
    return 0;
}

void Controler::_TransferData(const QVector<quint32> data, const Movement direction) {
    qDebug()<<"_TransferData";

    EMGdata EMGelement;
    EMGelement.movementIndex = direction;

    EMGelement.electrodeEMG = data;

    _EMGresults.push_back(EMGelement);

//    for(unsigned int i=0; i<INTERRUPTNUM; i++){
//        EMGdata EMGelement;
//        _EMGresults.push_back(EMGelement);
//        _EMGresults.back().movementIndex = direction;

//        for(unsigned int k=0; k<24; k+=4){
//            quint32 uiValue = 0;
//            for (int j=0; j!=4; j++) {
//                uiValue |= data[i*32+k+j] << (24 - j*8);
//                qDebug()<<i*32+k+j;
//                //qDebug()<<reslutsArray[i*k+j]<<" "<<(reslutsArray[i*k+j] << (24 - j*8));
//            }
//            //qDebug()<<reslutsArray[i*k]<<" "<<reslutsArray[i*k+1]<<" "<<reslutsArray[i*k+2]<<" "<<reslutsArray[i*k+3]<<" "<<uiValue;
//            _EMGresults.back().electrodeEMG.push_back(uiValue);
//        }
//    }
}

void Controler::ShowResults(){
    for (int i = 0; i < _EMGresults.size(); i++) {
        qDebug()<<i<<": "<<_EMGresults[i].electrodeEMG[0]<<" "
                         <<_EMGresults[i].electrodeEMG[1]<<" "
                         <<_EMGresults[i].electrodeEMG[2]<<" "
                         <<_EMGresults[i].electrodeEMG[3]<<" "
                         <<" value: "<<_EMGresults[i].movementIndex;
    }
}
