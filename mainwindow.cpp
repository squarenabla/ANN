#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <libusb-1.0/libusb.h>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    ui->setupUi(this);

    QTableWidgetItem *header = new QTableWidgetItem();
    header->setText("Neuron number");
    ui->tableWidget->setHorizontalHeaderItem(0,header);

    QTableWidgetItem *item = new QTableWidgetItem("1");
    ui->tableWidget->setItem(0,0, item);

    _tableDelegate = new Delegate;
    ui->tableWidget->setItemDelegate(_tableDelegate);


  //  usbDevice = new Device();
  //  usbDevice->Init();
    //connect(ui->);
}

MainWindow::~MainWindow(){
    delete _tableDelegate;
    delete ui;
   // delete usbDevice;
}

void MainWindow::on_pushButton_clicked(){
    //ui->tableWidget->insertRow(1);
    _controler.Move(UP);
    _controler.ShowResults();
    _controler.Teach();
}

void MainWindow::on_spinBox_valueChanged(int arg1){
    qint32 rowNum =  ui->tableWidget->rowCount();

    for(qint32 i=0; i<arg1-rowNum; i++){
        ui->tableWidget->insertRow(rowNum+i);
        QTableWidgetItem *item = new QTableWidgetItem("1");
        ui->tableWidget->setItem(rowNum+i,0, item);
    }

    for(qint32 i=0; i<rowNum-arg1; i++)
        ui->tableWidget->removeRow(rowNum-1-i);

}
