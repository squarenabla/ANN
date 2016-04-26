#ifndef DATATYPE_H
#define DATATYPE_H

#define FANN_FILE_NAME "./data/fann"
#define POINTER_IMGSRC "./data/pointer.png"

#define ELECTRODE_NUM 8

#define INTERRUPT_NUM 5
#define DATA_SIZE 4

#define TRAIN_NUM 5
#define MOVE_NUM 5
#define ANN_INPUT_SIZE ELECTRODE_NUM
#define BUFFER_SIZE 512
//#define MAX_AML_VALUE UINT_MAX
#define MAX_AML_VALUE 10


#include <QObject>
#include <QVector>

#include <QDebug>
#include <QItemDelegate>
#include <QLineEdit>

#include <algorithm>
#include <vector>
//#include <QtAlgorithms>
//#include "fann.h"

#include "messages.h"

typedef QVector<quint16> ANNLayers;
typedef QVector<qreal> ElectrodeEMG;
typedef QVector<qreal> FourierTransform;
typedef QVector<qreal> WaveletTransform;
typedef QVector<QVector<qreal> > EMGFourier;
typedef QVector<QVector<qreal> > EMGWavelet;
typedef QVector<qreal> EMGRMS;
typedef QVector<EMGFourier> EMGFourierVec;
typedef QVector<EMGWavelet> EMGWaveletVec;
typedef QVector<EMGRMS> EMGRMSVec;

typedef enum Movement {
    REST = 0,
    UP = 1,
    DOWN = 2,
    RIGHT = 3,
    LEFT = 4
} Movement;

struct TrainData {
    QVector<quint16> input;
    QVector<quint16> output;
};

struct EMGdata {
    ElectrodeEMG electrodeEMG;
    quint32 movementIndex;
};

struct EMGFourierData {
    QVector<QVector<qreal> > fourierArray;
    quint32 movementIndex;
};

struct EMGWaveletData {
    QVector<QVector<qreal> > waveletArray;
    quint32 movementIndex;
};

struct EMGRMSData {
    QVector<qreal> rmsArray;
    quint32 movementIndex;
};

struct EMGcharacteristics {
    EMGcharacteristics() : fourier(ELECTRODE_NUM), wavelet(ELECTRODE_NUM), rms(ELECTRODE_NUM) {}
    QVector<QVector<qreal> > fourier;
    QVector<QVector<qreal> > wavelet;
    QVector<qreal> rms;
    quint32 movementIndex;
};

class Delegate : public QItemDelegate {
public:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                      const QModelIndex & ndex) const {
        QLineEdit *lineEdit = new QLineEdit(parent);
        // Set validator
        QIntValidator *validator = new QIntValidator(1, 100, lineEdit);
        lineEdit->setValidator(validator);
        lineEdit->setText("1");
        return lineEdit;
    }
};


const static Movement trainSequence[TRAIN_NUM] = {REST, UP, DOWN, LEFT, RIGHT};
//const static Movement trainSequence[TRAIN_NUM] = {UP, UP, UP, UP, UP, UP, UP, UP, UP, UP};

template <class Type>
void printVector(QVector<Type> vec) {
    for (int i = 0; i < vec.size(); i++) {
        qDebug() << vec[i];
    }
}

#endif // DATATYPE_H
