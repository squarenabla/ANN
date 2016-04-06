#include "view.h"
#include "ui_main.h"
//#include <libusb-1.0/libusb.h>




View::View(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::View){
    ui->setupUi(this);

    QTableWidgetItem *header = new QTableWidgetItem();
    header->setText("Neuron number");
    ui->tableWidget->setHorizontalHeaderItem(0, header);

    QTableWidgetItem *item = new QTableWidgetItem("1");
    ui->tableWidget->setItem(0, 0, item);

    _tableDelegate = new Delegate;
    ui->tableWidget->setItemDelegate(_tableDelegate);


  //  usbDevice = new Device();
  //  usbDevice->Init();
    //connect(ui->);
}

View::~View(){
    delete _tableDelegate;
    delete ui;
   // delete usbDevice;
}

void View::on_pushButton_clicked(){
    //ui->tableWidget->insertRow(1);
    qDebug()<<"pb clicked";
    if(!_validateTable()){
        _ShowError(BAD_TAB_CONTENT);
    }
    else{
        _controler.Move(UP);
        _controler.ShowResults();
        _controler.Teach();
    }
}

void View::on_spinBox_valueChanged(int arg1){
    qint32 rowNum =  ui->tableWidget->rowCount();

    for(qint32 i = 0; i < arg1-rowNum; i++){
        ui->tableWidget->insertRow(rowNum + i);
        QTableWidgetItem *item = new QTableWidgetItem("1");
        ui->tableWidget->setItem(rowNum + i, 0, item);
    }

    for(qint32 i = 0; i < rowNum-arg1; i++)
        ui->tableWidget->removeRow(rowNum - 1 - i);

}

void View::_ShowError(QString errString){
    Error err(this, errString);
    err.show();
}

bool View::_validateTable(){
    for(int i = 0; i < ui->tableWidget->rowCount(); i++){
        if(ui->tableWidget->item(0,0)->text() == ""){
            return 0;
        }
    }
    return 1;
}
