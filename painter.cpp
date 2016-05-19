#include "painter.h"

Painter::Painter(QWidget *parent) :
QWidget(parent), xPos(0), yPos(0) {
    if (!image.load("./data/cross")) {
        qDebug() << "Error loading image";
    }
    rotateAngl = 0.0;
}

void Painter::SetImage(const QString &fileName) {
    image.load(fileName);
    repaint();
}

void Painter::MoveImage(const Movement direction) {
    switch (direction) {
    case UP:
        yPos = qMax((const int)--yPos, -height()/2);
        break;
    case DOWN:
        yPos = qMin((const int)++yPos, height()/2);
        break;
    case LEFT:
        xPos = qMax((const int)--xPos, -width()/2);
        break;
    case RIGHT:
        xPos = qMin((const int)++xPos, width()/2);
        break;
    default:
        break;
    }
    repaint();
}

void Painter::RotateImage(const Movement direction) {
    switch (direction) {
    case REST:
        image.load("./data/cross");
        break;
    case UP:
        image.load("./data/arrowU");
        break;
    case DOWN:
        image.load("./data/arrowD");
        break;
    case RIGHT:
        image.load("./data/arrowR");
        break;
    case LEFT:
        image.load("./data/arrowL");
        break;
    default:
        break;
    }
    repaint();
}

void Painter::paintEvent(QPaintEvent *e) {
    QPainter painter (this);
    int imagew = image.width()/10;
    int imageh = image.height()/10;
    QRectF target(0.0, 0.0, imagew, imageh);
    QRectF source(0.0, 0.0, image.width(), image.height());

    QTransform matrix;
    //matrix.rotate(rotateAngl);
    matrix.translate((width() - imagew)/2 + xPos, (height() - imageh)/2 + yPos);

    painter.setWorldTransform(matrix);

    painter.drawImage(target, image, source);
}
