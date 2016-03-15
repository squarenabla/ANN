#include "tablewidget.h"

TableWidget::TableWidget(QWidget *parent) :
    QTableWidget(parent)
{
}

void TableWidget::insertRows(int row){
    this->insertRow(1);
    int rowNum = this->rowCount();
    int columnNum = this->colorCount();
    QTableWidgetItem *newItem = new QTableWidgetItem();
    this->setItem(rowNum, columnNum, newItem);
//    //for(unsigned int i=)
}
