#include "view.h"
#include "ui_main.h"
//#include <libusb-1.0/libusb.h>

View::View(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::View) {
    ui->setupUi(this);

    QTableWidgetItem *header = new QTableWidgetItem();
    header->setText("Neuron number");
    ui->tableWidget->setHorizontalHeaderItem(0, header);

    QTableWidgetItem *item = new QTableWidgetItem("1");
    ui->tableWidget->setItem(0, 0, item);

    _tableDelegate = new Delegate;
    ui->tableWidget->setItemDelegate(_tableDelegate);

    setupPlot(ELECTRODE_NUM);

    _controler = new Controler();
    _controler->moveToThread(&workerThread);

    ui->executionWidget->SetImage(POINTER_IMGSRC);

    //connect(ui->executeButton, SIGNAL(pressed()), _controler, SLOT(Execute()));
    connect(this, SIGNAL(startLearning()), _controler, SLOT(Teach()), Qt::QueuedConnection);
    connect(_controler, SIGNAL(LearnMovementChanged(Movement)), ui->trainWidget, SLOT(RotateImage(Movement)));
    connect(_controler, SIGNAL(LearnMovementChanged(Movement)), this, SLOT(changeInstruction(Movement)));
    connect(_controler, SIGNAL(ExecuteMovementChanged(Movement)), ui->executionWidget, SLOT(MoveImage(Movement)));
    connect(_controler, SIGNAL(DataReceived(quint32,ElectrodeEMG)), this, SLOT(plotEMGData(quint32,ElectrodeEMG)));
    connect(_controler, SIGNAL(FourierTranformation(FourierTransform)), this, SLOT(PlotFourierTransformation(FourierTransform)));
    connect(_controler, SIGNAL(WaveletTransformation(WaveletTransform)), this, SLOT(PlotWaveletTransformation(WaveletTransform)));
    connect(&workerThread, SIGNAL(finished()), _controler, SLOT(deleteLater()));
    workerThread.start();
}

View::~View() {
    delete _tableDelegate;
    delete ui;
    workerThread.quit();
    workerThread.wait();
}

void View::setupPlot(const quint16 graphNum) {
    ui->fourierGraph->addGraph();
    ui->fourierGraph->xAxis->setRange(0, BUFFER_SIZE);

    for (quint16 i = 0; i < graphNum; i++) {
        QColor color(qrand() % 256, qrand() % 256, qrand() % 256);
        ui->graph->addGraph();
        ui->graph->graph(i)->setPen(QPen(color));
    }

    for (quint16 i = graphNum; i < 2 * graphNum; i++) {
        ui->graph->addGraph();
        ui->graph->graph(i)->setPen(QPen(Qt::red));
        ui->graph->graph(i)->setLineStyle(QCPGraph::lsNone);
        ui->graph->graph(i)->setScatterStyle(QCPScatterStyle::ssDisc);
    }

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->graph->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->graph->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->graph->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->graph->yAxis2, SLOT(setRange(QCPRange)));
}

void View::plotEMGData(const quint32 key, const ElectrodeEMG &data) {
    for (int i = 0; i < data.size(); i++) {
        // add data to lines:
        ui->graph->graph(i)->addData((double)key, (double)data[i]);
        // set data of dots:
        ui->graph->graph(data.size() + i)->clearData();
        ui->graph->graph(data.size() + i)->addData((double)key, (double)data[i]);
        // remove data of lines that's outside visible range:
        ui->graph->graph(i)->removeDataBefore((double)key-300);

    }
    // rescale value (vertical) axis to fit the current data:
    ui->graph->graph(0)->rescaleValueAxis();
    for (int i = 1; i < data.size(); i++) {
        ui->graph->graph(i)->rescaleValueAxis(true);
    }

    // make key axis range scroll with the data (at a constant range size of 8):
    ui->graph->xAxis->setRange((double)key+0.25, 300, Qt::AlignRight);
    ui->graph->replot();
}

void View::PlotFourierTransformation(FourierTransform yData) {
    QVector<double> xData(yData.size());

    for (int i = 0; i < yData.size(); i++)
        xData[i] = i;

    ui->fourierGraph->graph(0)->setData(xData, yData);
    ui->fourierGraph->graph(0)->rescaleValueAxis();
    ui->fourierGraph->replot();
}

void View::PlotWaveletTransformation(const WaveletTransform &yData) {
    QVector<double> xData(yData.size());

    for (int i = 0; i < yData.size(); i++)
        xData[i] = i;

    ui->fourierGraph->graph(0)->setData(xData, yData);
    ui->fourierGraph->graph(0)->rescaleValueAxis();
    ui->fourierGraph->replot();
}

void View::on_spinBox_valueChanged(int arg1) {
    qint32 rowNum =  ui->tableWidget->rowCount();

    for (qint32 i = 0; i < arg1-rowNum; i++) {
        ui->tableWidget->insertRow(rowNum + i);
        QTableWidgetItem *item = new QTableWidgetItem("1");
        ui->tableWidget->setItem(rowNum + i, 0, item);
    }

    for (qint32 i = 0; i < rowNum - arg1; i++)
        ui->tableWidget->removeRow(rowNum - 1 - i);

}

void View::_ShowError(QString errString) {
    Error err(this, errString);
    err.show();
}

bool View::_ValidateTable() {
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        if (ui->tableWidget->item(0,0)->text() == "") {
            return 0;
        }
    }
    return 1;
}

void View::on_learnButton_clicked() {
    //ui->tableWidget->insertRow(1);
    qDebug()<<"pb clicked";

    ui->trainWidget->RotateImage(UP);
    ANNLayers hiddenLayers(ui->tableWidget->rowCount());
    for (int i = 0; i < hiddenLayers.size(); i++) {
        hiddenLayers[i] = ui->tableWidget->item(i,0)->text().toUInt();
    }

    _controler->SetANNLayers(hiddenLayers);
    emit startLearning();
}

void View::changeInstruction(const Movement movement) {
    switch (movement) {
    case REST:
        ui->comandLabel->setText("Please, perform a rest");
        break;
    case UP:
        ui->comandLabel->setText("Please, perform an UP movement");
        break;
    case DOWN:
        ui->comandLabel->setText("Please, perform a DOWN movement");
        break;
    case RIGHT:
        ui->comandLabel->setText("Please, perform a RIGHT movement");
        break;
    case LEFT:
        ui->comandLabel->setText("Please, perform a LEFT movement");
        break;
    default:
        break;
    }
}

//============================
//-------HardCode Test--------
//============================

void View::on_upBtn_clicked() {
    _controler->Execute(UP);
}

void View::on_restBtn_clicked() {
    _controler->Execute(REST);
}

void View::on_downBtn_clicked() {
    _controler->Execute(DOWN);
}

void View::on_leftBtn_clicked() {
    _controler->Execute(LEFT);
}

void View::on_rightBtn_clicked() {
    _controler->Execute(RIGHT);
}


void View::on_executeButton_clicked() {
    _controler->StartRTExecution();
}

void View::on_stopBtn_clicked() {
    _controler->StopRTExecution();
}

