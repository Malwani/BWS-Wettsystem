#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);

    QObject::connect(&clientHandler, SIGNAL(uebermittleLogNachricht(QString)), this, SLOT(takeNewData(QString)));
    this->ui->edtIP->setText("127.0.0.1");
    this->ui->edtPort->setText("8888");
}

Client::~Client()
{
    delete ui;
}

void Client::on_btnVerbinden_clicked()
{
    if(ui->edtIP->text().isEmpty() || ui->edtPort->text().isEmpty())
    {
        ui->lwOutput->addItem("Bitte Host & Port angeben.");
    }
    else
    {
        clientHandler.setHost(this->ui->edtIP->text());
        clientHandler.setPort(this->ui->edtPort->text().toInt());

        if(!this->clientHandler.doConnect())
        {
            this->ui->lwOutput->addItem(QString("Fehler:") + this->clientHandler.getLastError());
        }
    }
}

void Client::on_btnSenden_clicked()
{
    QString data = this->ui->edtSenden->text();
    this->clientHandler.sendData(data);
    this->ui->lwOutput->addItem(QString("Gesendet: ") + data);
}

void Client::takeNewData(QString data)
{
    ui->lwOutput->addItem(data);
}
