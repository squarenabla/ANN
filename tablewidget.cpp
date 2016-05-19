#include "tablewidget.h"

TableWidget::TableWidget(QWidget *parent) :
QTableWidget(parent) {}

void TableWidget::insertRows(int row) {
    insertRow(1);
    int rowNum = rowCount();
    int columnNum = colorCount();
    QTableWidgetItem *newItem = new QTableWidgetItem();
    setItem(rowNum, columnNum, newItem);
}
