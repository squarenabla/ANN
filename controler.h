#ifndef CONTROLER_H
#define CONTROLER_H



#include <QObject>

#include "types.h"
#include "teacher.h"
#include "device.h"



class Controler : public QObject{
    Q_OBJECT
public:
    explicit Controler(QObject *parent = 0);

signals:

public slots:
    int Move(const Movement direction);
    int SetTeacher();
    int Teach();

    void ShowResults();

private:
    void _TransferData(const QVector<quint32> data, const Movement direction);

    Teacher _teacher;
    Device  _usbDevice;
    QVector<EMGdata> _EMGresults;
};

#endif // CONTROLER_H
