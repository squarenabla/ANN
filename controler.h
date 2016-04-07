#ifndef CONTROLER_H
#define CONTROLER_H



#include <QObject>

#include "types.h"
#include "teacher.h"
#include "executor.h"
#include "device.h"



class Controler : public QObject{
    Q_OBJECT
public:
    explicit Controler(QObject *parent = 0);

signals:

public slots:

    int SetTeacher();
    int SetANNLayers(const ANNLayers hiddenLayers);
    void Teach();
    void Execute();

    void ShowResults();

private:
    int _Move(const Movement direction);
    void _TransferData(const QVector<quint32> data, const Movement direction);


    Teacher _teacher;
    Executor _executor;
    Device  _usbDevice;
    QVector<EMGdata> _EMGresults;
};

#endif // CONTROLER_H
