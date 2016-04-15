/*
  Modul: verbindungsaufbau.cpp
  Funktion: 
  Autor: Christoph Rehm
  Datum: 
  */

#include "verbindungsaufbau.h"
#include "ui_verbindungsaufbau.h"

#include <QDesktopServices>
#include <QUrl>


Verbindungsaufbau::Verbindungsaufbau(QWidget *parent) : // Autor: Christoph Rehm
    QWidget(parent),
    ui(new Ui::Verbindungsaufbau)
{
    ui->setupUi(this);

    QDomDocument docXmlVerbindung;

    // xml Datei Pfad angeben
    QFile fileXmlVerbindung("verbindungsaufbau.xml");
    // xml Datei öffnen
    if(!fileXmlVerbindung.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "xml Datei konnte nicht geöffnet werden!";

        // xml Datei erstellen
        if(!this->xmlFileErstellen())
            qDebug() << "xml Datei konnte nicht erstellt werden!";
        else
            qDebug() << "xml Datei wurde erstellt!";
    }
    else
    {
        // Daten aus der xml Datei laden
        docXmlVerbindung.setContent(&fileXmlVerbindung);

        // root laden
        QDomElement root = docXmlVerbindung.firstChildElement();

        // ip und port von "LetzterVerbindung" laden
        QString ip = root.firstChildElement().attribute("IP","");
        QString port = root.firstChildElement().attribute("Port","");

        // ip und port in die Text felder schreiben
        ui->edtIP->setText(ip);
        ui->edtPort->setText(port);

        // xml Datei schließen
        fileXmlVerbindung.close();

    }

}

bool Verbindungsaufbau::xmlFileErstellen() // Autor: Christoph Rehm
{
    bool ok = true;

    // zu schreibendes Dokument erstellen
    QDomDocument docXmlVerbindung;

    //root Element erstellen
    QDomElement root = docXmlVerbindung.createElement("Verbindung");
    docXmlVerbindung.appendChild(root);

    //Element "LetzteVerbindung" erstellen
    QDomElement letzteVerbindung = docXmlVerbindung.createElement("LetzteVerbindung");
    //attribute erstellen
    letzteVerbindung.setAttribute("Port", "");
    letzteVerbindung.setAttribute("IP","");
    //Element "LetzteVerbindung" als Unterelement von root hinzufügen
    root.appendChild(letzteVerbindung);

    // xml Datei erstellen und öffnen
    QFile fileXmlVerbindung("verbindungsaufbau.xml");
    ok = fileXmlVerbindung.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!ok)
    {
        qDebug() << "xml Datei konnte nicht erstellt werden!";
    }
    else
    {
        //Daten in die xml Datei Schreiben
        fileXmlVerbindung.write(docXmlVerbindung.toByteArray(2));

        //xml Datei schließen
        fileXmlVerbindung.close();
    }

    return ok;
}

Verbindungsaufbau::~Verbindungsaufbau() // Autor: Christoph Rehm
{
    delete ui;
}

void Verbindungsaufbau::showWindow(bool mode) // Autor: Christoph Rehm
{
    this->setVisible(mode);
}

void Verbindungsaufbau::on_btnVerbinden_clicked() // Autor\en: Christoph Rehm
{
    QString host = ui->edtIP->text();
    QString port = ui->edtPort->text();

    // Dokument erstellen
    QDomDocument docXmlVerbindung;

    // xml Datei Pfad angeben
    QFile fileXmlVerbindung("verbindungsaufbau.xml");
    // xml Datei öffnen
    if(!fileXmlVerbindung.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << "xml Datei konnte nicht geöfnet werden!";
    }
    else
    {

        // Daten aus der xml Datei laden
        docXmlVerbindung.setContent(&fileXmlVerbindung);

        // root laden
        QDomElement root = docXmlVerbindung.firstChildElement();

        // Attribute von "LetzterVerbindung" setzen
        root.firstChildElement().setAttribute("Port", port);
        root.firstChildElement().setAttribute("IP", host);

        //Daten in der xml Datei zurücksetzen
        fileXmlVerbindung.reset();
        //Daten in die xml Datei Schreiben
        fileXmlVerbindung.write(docXmlVerbindung.toByteArray(2));

        // xml Datei schließen
        fileXmlVerbindung.close();
    }

    this->p_clientHandler->setHost(host);
    this->p_clientHandler->setPort(port.toInt());

    if(!this->p_clientHandler->doConnect())
        QMessageBox::critical(this, "Verbindungsfehler", this->p_clientHandler->getLastError());
    else
    {
        QMessageBox::information(this, "Erfolg!", "Verbindung erfolgreich hergestellt.");
        emit openLogin(true);
        this->hide();
    }
}

void Verbindungsaufbau::takeClientHandler(ClientHandler* p_clientHandler)
{
    this->p_clientHandler = p_clientHandler;
}

void Verbindungsaufbau::on_btnInfo_clicked()
{
    i.show();
}

void Verbindungsaufbau::on_btnHilfe_clicked()
{
    QString path = QCoreApplication::applicationDirPath() + "/BWS_Wettsystem_Benutzerhandbuch_Client.pdf";

    QDesktopServices::openUrl(QUrl("file:" + path, QUrl::TolerantMode));
}
