#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login) {
    ui->setupUi(this);
}

Login::~Login() {
    delete ui;
}

void Login::on_pushButton_clicked()
{
    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QString device = "{\"email\": \"" + ui->login->text() + "\", \"password\": \"" + ui->pswd->text() + "\"}";

    QByteArray jsonString = device.toUtf8();

    QNetworkRequest request(QUrl("http://127.0.0.1:2599/api/auth/login"));
    request.setRawHeader("Content-Type", "application/json");

    QNetworkReply *reply = mgr.post(request, jsonString);

    eventLoop.exec();


    if (reply->error() == QNetworkReply::NoError) {
        QString successrpl(reply->readAll());
        qDebug() << "Success" <<successrpl;

        if(!successrpl.endsWith("/failure")) {
            emit loginSuccessful();
            qDebug() << "logged in";
        }
        else {
            ui->label_3->setText("Please, try again");
        }
        //ui->lineEdit_3->setText( reply->readAll() );
        delete reply;
    }
    else {
        qDebug() << "Failure" <<reply->errorString();
        ui->label_3->setText("No connection(");
        //ui->lineEdit_3->setText( reply->errorString() );
        delete reply;
    }

}
