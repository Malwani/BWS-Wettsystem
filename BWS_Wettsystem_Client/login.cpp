#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_btnLogin_clicked()
{
    QString benutzername = ui->edtBenutzername->text();
    QString passwort = QString(QCryptographicHash::hash(ui->edtPasswort->text().toUtf8(),QCryptographicHash::Md5).toHex());


    if(benutzername.isEmpty() || passwort.isEmpty())
        QMessageBox::information(this,"Information", "Bitte Login-Daten eingeben.");
    else  // sonst normalen login senden
    {
        this->p_clientHandler->activeUser.setUser(benutzername);
        this->p_clientHandler->sendData(this->p_clientHandler->protokoll.packCR_LOGIN(benutzername, passwort));
    }
}

void Login::showWindow(bool mode)
{
    if(!mode)
    {
        this->ui->edtBenutzername->clear();
        this->ui->edtPasswort->clear();
    }

    this->setVisible(mode);
}

void Login::on_btnRegistrieren_clicked()
{
    //if(Ich würd da noch irgend eine Methode zum Netzwerk check machen...)
    emit openRegister(true);
}

void Login::on_pushButton_clicked()
{
    this->hide();
    this->openVerbindungsaufbau(true);
}

void Login::takeClientHandler(ClientHandler *p_clientHandler)
{
    this->p_clientHandler = p_clientHandler;
}
