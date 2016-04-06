#include "error.h"
#include "ui_error.h"

Error::Error(QWidget *parent, QString errString) :
QDialog(parent),
ui(new Ui::Error){

    ui->setupUi(this);
    ui->label->setText(errString);
}

Error::~Error(){
    delete ui;
}
