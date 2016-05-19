#ifndef TEACHER_H
#define TEACHER_H

#include "types.h"

#include "fann.h"

class Teacher: public QObject
{
    Q_OBJECT

public:
    explicit Teacher(QObject *parent = 0);
    //~Teacher();

signals:

public slots:
    void setLayers(quint16 num_layers, const quint16 *layers);

    void createTrainData(const QVector<EMGdata> data);
    void trainOnFile(const char *fileName);
    void trainOnData();
    //void trainOnData(TrainData trainData);

private:
    quint16                 _num_input;
    QVector<quint16>        _numNeuronsInLayer;
    quint16                 _num_layers;
    qreal                   _desired_error;
    quint16                 _max_epochs;
    quint16                 _epochs_between_reports;
    struct fann_train_data *_data;

};

#endif // TEACHER_H
