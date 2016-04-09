#include "painter.h"

Painter::Painter(QWidget *parent) : QWidget(parent) {
    if (!image.load("./data/arrowR")) {
        qDebug() << "Error loading image";
    }
    rotateAngl = 0.0;
}

void Painter::rotateImage(const Movement direction) {
    switch (direction) {
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

void Painter::paintEvent ( QPaintEvent *e ) {
    QPainter painter (this);
    int imagew = image.width()/10;
    int imageh = image.height()/10;
    QRectF target(0.0, 0.0, imagew, imageh);
    QRectF source(0.0, 0.0, image.width(), image.height());

    QTransform matrix;
    //matrix.rotate(rotateAngl);
    matrix.translate((width() - imagew)/2, (height() - imageh)/2);

    painter.setWorldTransform(matrix);

    painter.drawImage(target, image, source);
}
