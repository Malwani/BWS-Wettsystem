#include "register.h"
#include "ui_register.h"

Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
}

Register::~Register()
{
    delete ui;
}

void Register::showWindow(bool mode)
{
    this->setVisible(mode);
}

void Register::takeClientHandler(ClientHandler *p_clientHandler)
{
    this->p_clientHandler = p_clientHandler;
}

void Register::registerScucced()
{
    this->ui->edtBenutzername->clear();
    this->ui->edtEmail->clear();
    this->ui->edtKlasse->clear();
    this->ui->edtNachname->clear();
    this->ui->edtPasswort->clear();
    this->ui->edtPasswortWiederholt->clear();
    this->ui->edtVorname->clear();

    this->hide();
}

void Register::on_btnRegistrieren_clicked()
{

    QString username = this->ui->edtBenutzername->text();
    QString passwort = this->ui->edtPasswort->text();
    QString passwortWiederholt = this->ui->edtPasswortWiederholt->text();
    QString email = this->ui->edtEmail->text();
    QString vorname = this->ui->edtVorname->text();
    QString nachname = this->ui->edtNachname->text();
    QString klasse = this->ui->edtKlasse->text();

    QString hashedPass = QString(QCryptographicHash::hash(passwort.toUtf8(),QCryptographicHash::Md5).toHex());

    if( username.isEmpty() || email.isEmpty() || vorname.isEmpty() || nachname.isEmpty() || klasse.isEmpty() || passwort.isEmpty() ) // Wenn Felder leer
        QMessageBox::information(this,"Information", "Bitte vollständige Accountdaten angeben.");
    else
    {
        if(!email.contains("@"))
            QMessageBox::information(this,"Information", "Email ungültig.");
        else
        {
            if(passwort != passwortWiederholt)
                QMessageBox::information(this,"Information", "Ihr Passwort stimmt nicht überein,");
            else
            {
                QString cr_reg = this->p_clientHandler->protokoll.packCR_REG(username, hashedPass,email,vorname,nachname,klasse);
                this->p_clientHandler->sendData(cr_reg);
            }
        }
    }
}
