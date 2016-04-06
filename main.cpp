#include "view.h"
#include <QApplication>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    View window;
    window.show();

    return a.exec();
}
