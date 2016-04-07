#ifndef DATATYPE_H
#define DATATYPE_H

#define FANNFILENAME "./data/fann"

#define ELECTRODENUM 4

#define INTERRUPTNUM 5
#define DATASIZE 4

#define TRAINNUM 10

//#define MAXAMLVALUE UINT_MAX
#define MAXAMLVALUE 10

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

enum Movement{
    REST = 0,
    UP = 2,
    DOWN = 4,
    RIGHT = 6,
    LEFT = 8
};

struct TrainData{
    QVector<quint16> input;
    QVector<quint16> output;
};

struct EMGdata{
    QVector<quint32> electrodeEMG;
    quint32 movementIndex;
};

class Delegate : public QItemDelegate{
public:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                      const QModelIndex & ndex) const
    {
        QLineEdit *lineEdit = new QLineEdit(parent);
        // Set validator
        QIntValidator *validator = new QIntValidator(1, 9, lineEdit);
        lineEdit->setValidator(validator);
        lineEdit->setText("1");
        return lineEdit;
    }
};


//Movement trainSequence[TRAINNUM] = {UP, DOWN, RIGHT, LEFT, REST, UP, DOWN, RIGHT, LEFT, REST};
const static Movement trainSequence[TRAINNUM] = {UP, UP, UP, UP, UP, UP, UP, UP, UP, UP};

template <class Type>
void printVector(QVector<Type> vec){
    for(int i = 0; i < vec.size(); i++){
        qDebug() << vec[i];
    }
}

#endif // DATATYPE_H
