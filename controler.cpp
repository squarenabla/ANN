#include "controler.h"

Controler::Controler(QObject *parent) : QObject(parent){
    _usbDevice.Init();
}

int Controler::SetTeacher(){
    return 0;
}

int Controler::Teach(){
    _teacher.createTrainData(_EMGresults);
    _teacher.trainOnData();
    return 0;
}

int Controler::Move(const Movement direction){
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

    for(unsigned int i = 0; i < INTERRUPTNUM; ++i){
        QVector<quint32> data(DATASIZE);
        _usbDevice.Interrupt(data);
        _TransferData(data, direction);
    }
    
    return 0;
}

void Controler::_TransferData(const QVector<quint32> data, const Movement direction){
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
    for(unsigned int i = 0; i < _EMGresults.size(); i++){
        qDebug()<<i<<": "<<_EMGresults[i].electrodeEMG[0]<<" "
                         <<_EMGresults[i].electrodeEMG[1]<<" "
                         <<_EMGresults[i].electrodeEMG[2]<<" "
                         <<_EMGresults[i].electrodeEMG[3]<<" "
                         <<" value: "<<_EMGresults[i].movementIndex;
    }
}
