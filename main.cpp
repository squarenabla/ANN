#include "view.h"
#include "types.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    qRegisterMetaType<Movement>("Movement");
    qRegisterMetaType<ElectrodeEMG>("ElectrodeEMG");
    qRegisterMetaType<FourierTransform>("FourierTransform");
    QApplication a(argc, argv);
    View window;
    window.show();
    return a.exec();
}
