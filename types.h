#ifndef DATATYPE_H
#define DATATYPE_H

#define FANNFILENAME "./data/fann"

#define ELECTRODENUM 4

#define INTERRUPTNUM 5
#define DATASIZE 4

#include <QObject>
#include <QVector>

#include <QDebug>
#include <QItemDelegate>
#include <QLineEdit>

#include "messages.h"

struct TrainData{
    QVector<quint16> input;
    QVector<quint16> output;
};


enum Movement{
    UP      = 1,
    DOWN    = 2,
    RIGHT   = 3,
    LEFT    = 4
};


struct EMGdata{
    QVector<quint32> electrodeEMG;
    quint32 movementIndex;
};

class Delegate : public QItemDelegate{
public:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem & option,
                      const QModelIndex & index) const
    {
        QLineEdit *lineEdit = new QLineEdit(parent);
        // Set validator
        QIntValidator *validator = new QIntValidator(1, 9, lineEdit);
        lineEdit->setValidator(validator);
        lineEdit->setText("1");
        return lineEdit;
    }
};

#endif // DATATYPE_H
