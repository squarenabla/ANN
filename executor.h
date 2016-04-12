#ifndef EXECUTOR_H
#define EXECUTOR_H


#include "types.h"

#include "fann.h"

class Executor : public QObject {
    Q_OBJECT
public:
    explicit Executor(QObject *parent = 0);

signals:

public slots:
    void SetANN(const char *fileName);
    void Execute(const EMGFourierData &data, QVector<float> &output);

private:
    struct fann *_ann;
};

#endif // EXECUTOR_H
