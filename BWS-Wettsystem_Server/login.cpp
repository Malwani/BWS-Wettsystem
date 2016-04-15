#include "login.h"
#include "ui_login.h"


Login::Login(DBHandler *p_dbHandler, AccountListe* p_accountListe, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    this->p_dbHandler = p_dbHandler; // Pointer zum DBHandler wird übergeben
    this->p_accountListe = p_accountListe;
}

Login::~Login()
{
    delete ui;
}

void Login::on_btnLogin_clicked()
{
    QString benutzername = ui->edtBenutzername->text();
    QString passwort = ui->edtPasswort->text();
    QString hashedPasswort(QCryptographicHash::hash(passwort.toUtf8(),QCryptographicHash::Md5).toHex());
    QString error = "";
    if(!this->p_accountListe->LoginServer(benutzername, hashedPasswort, &error))
        QMessageBox::information(this,"Fehler", error);
    else
    {
        this->showWindow(false);
        emit this->openFrmMain(true);
        this->ui->edtBenutzername->clear();
        this->ui->edtPasswort->clear();
    }
}

void Login::showWindow(bool mode)
{
    this->setVisible(mode);
}

void Login::on_btnInfo_clicked() // Autor Christoph Rehm
{
    i.show();
}

void Login::on_btnHilfe_clicked()
{
    QString path = QCoreApplication::applicationDirPath();

    QDesktopServices::openUrl(QUrl("file:"+path+"/BWS_Wettsystem_Benutzerhandbuch_Server.pdf", QUrl::TolerantMode));
}
