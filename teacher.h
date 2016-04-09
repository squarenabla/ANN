#ifndef TEACHER_H
#define TEACHER_H

#include "types.h"

#include "fann.h"

class Teacher: public QObject {
    Q_OBJECT

public:
    explicit Teacher(QObject *parent = 0);
    //~Teacher();

signals:

public slots:
    void setLayers(const ANNLayers hiddenLayers);

    void createTrainData(const QVector<EMGdata> data);
    void trainOnFile(const char *fileName);
    void trainOnData();
    //void trainOnData(TrainData trainData);

private:
    quint16                 _numInput;
    QVector<quint16>        _numNeuronsInLayer;
    quint16                 _numLayers;
    qreal                   _desiredError;
    quint16                 _maxEpochs;
    quint16                 _epochsBetweenReports;
    struct fann_train_data *_data;

};

#endif // TEACHER_H
