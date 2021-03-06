#ifndef PAINTER_H
#define PAINTER_H

//#define IMAGEH 45
//#define IMAGEW 60
//#define IMAGESRC "./data/arrow.png"

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QMatrix>

#include <QDebug>

#include "types.h"

class Painter : public QWidget {
    Q_OBJECT
public:
    explicit Painter(QWidget *parent = 0);

signals:

public slots:
    void RotateImage(const Movement direction);
    void SetImage(const QString &fileName);
    void MoveImage(const Movement direction);
protected:
    void paintEvent(QPaintEvent *e);

    qreal rotateAngl;
    QImage image;
    qint16 xPos;
    qint16 yPos;
};

#endif // PAINTER_H
