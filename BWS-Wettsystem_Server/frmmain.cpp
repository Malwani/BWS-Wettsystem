#include "frmmain.h"
#include "ui_frmmain.h"


FrmMain::FrmMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmMain)
{
    ui->setupUi(this);

    ui->btnEinstellungManschaftenAktualisieren->setEnabled(false);
    ui->btnEinstellungEventsAktualisieren->setEnabled(false);
    ui->btnServerStoppen->setEnabled(false);
        this->ui->tabMain->setCurrentIndex(0);

    QString error;
    QSqlQuery p_qry;
    bool ok = true;

    this->p_dbHandler = new DBHandler();
    this->p_accountListe = new AccountListe(this->p_dbHandler); // AccountListe anlegen und pointer auf this(frmmain) & DBHandler übergeben
    this->p_login = new Login(this->p_dbHandler, this->p_accountListe); // Pointer zum DBHandler wird übergeben
    this->p_serverHandler = NULL;

    QObject::connect(this, SIGNAL(openLogin(bool)) , p_login, SLOT(showWindow(bool)) );
    QObject::connect(p_login, SIGNAL(openFrmMain(bool)), this, SLOT(showWindow(bool)) );

    // Laden der Mannschaften in QVector mannschaften und augabe in lwMannschaften   // Autor: Christoph Rehm
    //
    ok = true;
    if(this->p_dbHandler->mannschaften(&p_qry,&error))
    {
        if(p_qry.first())
        {
            while(ok)
            {
                mannschaften.append(p_qry.value(0).toString());
                ok = p_qry.next();
            }
        }
        this->ausgabeInLwMannschaften(&mannschaften);
    }
    else
        QMessageBox::warning(this,error,error);
    //
    //

    // tedt s auf das aktuelle Datum setzen   // Autor: Christoph Rehm
    //
    QDateTime tmpDateTime = QDateTime::currentDateTime();
    ui->tedtEinstellungenEventsStart->setDateTime(tmpDateTime);
    ui->tedtWettenErstellenStart->setDateTime(tmpDateTime);
    ui->tedtWettenVerwaltenStart->setDateTime(tmpDateTime);
    //
    //


    //Ausgabe der Einzelnen User
    if(!this->p_accountListe->aktualisieren(&error)) // hier wird die Adresse von Error übergeben, damit error am Ende eine mögliche Fehlernachricht enthält, die ausgegeben werden kann
        QMessageBox::critical(this,"Fehler!", error);
    else // ist alles gut gelaufen kann die Accountliste ausgegeben werden
        this->accountsAnzeigen();


    //Erstellen der EventListe und laden der Events in lwEvents   // Autor: Christoph Rehm
    p_eventListe = new EventListe(this->p_dbHandler,p_accountListe);
    this->ausgabeInLwEvents(p_eventListe);
    this->ladeNowWetten();
    //

    emit openLogin(true);

}

FrmMain::~FrmMain()
{
    delete ui;
}

void FrmMain::on_btnRegistrieren_clicked()//Autor: Aymane Amaioua
{
    QString vorname = ui->edtVorname->text();
    QString nachname = ui->edtNachname->text();
    QString email = ui->edtEmail->text();
    QString klasse = ui->edtKlasse->text();
    QString benutzer = ui->edtBenutzername->text();
    QString passwort = ui->edtPasswort->text();

    if(vorname.isEmpty() || nachname.isEmpty() || email.isEmpty() || klasse.isEmpty(), benutzer.isEmpty() || passwort.isEmpty())
        QMessageBox::information(this,"Information", "Bitte geben Sie vollständige Accountdaten an.");
    else
    {
        if(this->ui->edtPasswort->text() != this->ui->edtPasswort_2->text())
            QMessageBox::information(this,"Information","Ihr Passwort stimmt nicht überein.");
        else
        {
            QString error = "";
            QString willkommen = "Willkommen bei BWS-Wettsystem ihre Registrierung war erfolgreich!\nIhr Account muss nur noch von einem Admin Freigeschaltet werden dies könnte etwas dauern... :)";
            QString willkommen2 = "Willkommen Neuer Admin\nIhre Registrierung war Erfolgreich!\nIch hoffe sie haben Spaß am System :)";
            QString hashedPasswort(QCryptographicHash::hash(passwort.toUtf8(),QCryptographicHash::Md5).toHex());

            bool ok = ui->checkadmin->isChecked();

            if(!ok)
            {
               if(!this->p_accountListe->neuenAccountAnlegen(benutzer, hashedPasswort, vorname, nachname, email, klasse, &error)) //Checkt ob keine Fehler in der Abfrage oder in den Eingaben vorhanden ist
                QMessageBox::critical(this,"Fehler!",error);
                else
                {
                    QMessageBox::information(this, "Erfolgreich!", willkommen);//Info zur Erfolgreichen Registrierung
                    ui->edtBenutzername->clear();
                    ui->edtPasswort->clear();
                    ui->edtVorname->clear();
                    ui->edtNachname->clear();
                    ui->edtEmail->clear();
                    ui->edtKlasse->clear();
                    ui->edtPasswort_2->clear();
                }
            }
            else
            {
                if(!this->p_accountListe->AdminAnlegen(benutzer, hashedPasswort, &error))
                    QMessageBox::critical(this,"Fehler!",error);
                else
                {
                    QMessageBox::information(this, "Erfolgreich!", willkommen2);
                    ui->edtBenutzername->clear();
                    ui->edtPasswort->clear();
                }
            }
        }
    }
}

void FrmMain::on_tabUserVerwaltung_tabBarClicked(int index) // Autor: Aymane Amaioua
{
    if(index == 0) // Wenn in Verwaltung
        this->accountsAnzeigen();
}

void FrmMain::on_btnFreischalten_clicked()// Autor: Aymane Amaioua
{
    QString error = "";
    QString erfolgreich;

    if(this->ui->lwUserVerwalten->currentRow() == -1)
        QMessageBox::information(this, "Information","Es ist kein Nutzer ausgewählt."); // ist kein item gewählt?
    else
    {
        QString currentItemString = this->ui->lwUserVerwalten->currentItem()->text();

        QString benutzername = currentItemString.mid(0,currentItemString.indexOf(" "));

        bool freigeschaltet = true;

        if(this->ui->btnFreischalten->text() == "Freischalten")
            freigeschaltet = false;

        if(!freigeschaltet)
            erfolgreich = "Dieser Account wurde Erfolgreich freigeschaltet :)" ;
        else
            erfolgreich = "Dieser Account wurde Erfolgreich gesperrt :)";

        if(!this->p_accountListe->AccountFreischaltenSperren(freigeschaltet,benutzername,&error))
            QMessageBox::critical(this,"Fehler!",error);
        else
        {
            this->accountsAnzeigen();
            QMessageBox::information(this,"Erfolgreich!",erfolgreich);
        }
    }
}

void FrmMain::on_btnLogout_clicked()
{
    this->showWindow(false);
    emit openLogin(true);
}

void FrmMain::showWindow(bool mode)
{
    this->setVisible(mode);
}

void FrmMain::sortAnQVectorQString(QVector<QString> *vector) // Autor: Christoph Rehm
{
    int count = vector->count();
    bool sotiert = false;
    QString tmp;

    for (int y = 0; y < count-1 && !sotiert; y++)
    {
        sotiert = true;
        for (int i = 0; i < (count-1-y); i++)
        {
            if (vector->at(i).toUpper() > vector->at(i+1).toUpper())
            {
                tmp = vector->at(i);                //
                vector->replace(i,vector->at(i+1)); // Dreiekstausch
                vector->replace(i+1,tmp);           //
                sotiert = false;
            }
        }
    }
}

bool FrmMain::testStringIfInt(QString tmpString) // Autor: Christoph Rehm
{
    bool ok = true;

    for (int i = 0; i < tmpString.size(); i++)
    {
        if (!(tmpString.mid(i,1) >= "0" && tmpString.mid(i,1) <= "9"))
                ok = false;
    }

    return ok;
}

void FrmMain::on_btnEinstellungManschaftenEintragen_clicked() // Autor: Christoph Rehm
{
    QString mannschaftsName = ui->edtMannschaftsname->text();
    QString error;

    if (!mannschaften.contains(mannschaftsName))
    {
        if(this->p_dbHandler->eintragenMannschaft(mannschaftsName,&error))
        {
            mannschaften.append(mannschaftsName);
            this->ausgabeInLwMannschaften(&mannschaften);
        }
        else
        {
            QMessageBox::warning(this,error,error);
        }
    }
    else
    {
        QMessageBox::warning(this,"Eingabe existiert","Die Eingabe existiert bereits!");
    }

}


void FrmMain::ausgabeInLwMannschaften(QVector<QString> *tmp) // Autor: Christoph Rehm
{
    this->sortAnQVectorQString(tmp);

    ui->lwMannschaften->clear();

    ui->comboMannschaft1Erstellen->clear();
    ui->comboMannschaft1Erstellen->addItem("Bitte wählen");
    ui->comboMannschaft1Erstellen->addItem("Neue hinzufügen");

    ui->comboMannschaft2Erstellen->clear();
    ui->comboMannschaft2Erstellen->addItem("Bitte wählen");
    ui->comboMannschaft2Erstellen->addItem("Neue hinzufügen");

    ui->comboManschaft1Verwaltung->clear();
    ui->comboManschaft1Verwaltung->addItem("Bitte wählen");
    ui->comboManschaft1Verwaltung->addItem("Neue hinzufügen");

    ui->comboManschaft2Verwaltung->clear();
    ui->comboManschaft2Verwaltung->addItem("Bitte wählen");
    ui->comboManschaft2Verwaltung->addItem("Neue hinzufügen");

    QString name;

    for (int i = 0; i < tmp->count(); i++)
    {
        name = tmp->at(i);
        ui->lwMannschaften->addItem(name);
        ui->comboMannschaft1Erstellen->addItem(name);
        ui->comboMannschaft2Erstellen->addItem(name);
        ui->comboManschaft1Verwaltung->addItem(name);
        ui->comboManschaft2Verwaltung->addItem(name);
    }

}

void FrmMain::ausgabeInLwEvents(EventListe *tmp) // Autor: Christoph Rehm
{
    ui->lwEvents->clear();

    ui->comboEventErstellen->clear();
    ui->comboEventErstellen->addItem("Bitte wählen");
    ui->comboEventErstellen->addItem("Neue hinzufügen");

    ui->comboEventVerwaltungSort->clear();
    ui->comboEventVerwaltungSort->addItem("Bitte wählen");

    ui->comboUserVerwaltenEvent->clear();
    ui->comboUserVerwaltenEvent->addItem("Bitte wählen");

    QString name;

    for (int i = 0; i < tmp->count(); i++)
    {
        name = tmp->holen(i)->getName();
        ui->lwEvents->addItem(name);
        ui->comboEventErstellen->addItem(name);
        ui->comboEventVerwaltungSort->addItem(name);
        ui->comboUserVerwaltenEvent->addItem(name);
    }
}

void FrmMain::on_btnEinstellungManschaftenLoeschen_clicked() // Autor: Christoph Rehm
{
    QString error;
    ui->btnEinstellungManschaftenLoeschen->setEnabled(false);
    ui->btnEinstellungManschaftenAktualisieren->setEnabled(false);

    if (p_dbHandler->loeschenMannschaft(mannschaften.at(ui->lwMannschaften->currentRow()),&error))
    {
        mannschaften.removeAt(ui->lwMannschaften->currentRow());
        this->ausgabeInLwMannschaften(&mannschaften);
    }
    else
    {
        QMessageBox::warning(this,error,error);
    }

}

void FrmMain::on_lwMannschaften_itemClicked(QListWidgetItem *item) // Autor: Christoph Rehm
{
    ui->btnEinstellungManschaftenLoeschen->setEnabled(true);
    ui->btnEinstellungManschaftenAktualisieren->setEnabled(true);

    ui->edtMannschaftsname->setText(item->text());
}

void FrmMain::on_btnEinstellungManschaftenAktualisieren_clicked() // Autor: Christoph Rehm
{
    QString error;
    ui->btnEinstellungManschaftenLoeschen->setEnabled(false);
    ui->btnEinstellungManschaftenAktualisieren->setEnabled(false);

    if (p_dbHandler->updateMannschaft(ui->lwMannschaften->currentItem()->text(),ui->edtMannschaftsname->text(),&error))
    {
        mannschaften.replace(ui->lwMannschaften->currentRow(),ui->edtMannschaftsname->text());
        this->ausgabeInLwMannschaften(&mannschaften);
    }
    else
    {
        QMessageBox::warning(this,error,error);
    }
}



void FrmMain::on_comboManschaft1Verwaltung_currentIndexChanged(const QString &arg1) // Autor: Christoph Rehm
{
    if (arg1 == "Neue hinzufügen")
    {
        ui->tabMain->setCurrentIndex(3);
        ui->tabEinstellungen->setCurrentIndex(1);
        ui->comboManschaft1Verwaltung->setCurrentIndex(0);
    }
}

void FrmMain::on_comboManschaft2Verwaltung_currentIndexChanged(const QString &arg1) // Autor: Christoph Rehm
{
    if (arg1 == "Neue hinzufügen")
    {
        ui->tabMain->setCurrentIndex(3);
        ui->tabEinstellungen->setCurrentIndex(1);
        ui->comboManschaft2Verwaltung->setCurrentIndex(0);
    }
}


void FrmMain::on_comboEventErstellen_currentIndexChanged(const QString &arg1) // Autor: Christoph Rehm
{
    int index;

    if (arg1 == "Neue hinzufügen")
    {
        ui->tabMain->setCurrentIndex(3);
        ui->tabEinstellungen->setCurrentIndex(1);
        ui->comboEventErstellen->setCurrentIndex(0);
    }
    else if(arg1 == "Bitte wählen" || arg1 == "")
    {

    }
    else // Wenn ein event ausgewählt wurde
    {
        for (int i = 0; i < p_eventListe->count(); i++)
        {
            if (p_eventListe->holen(i)->getName() == arg1)
            {
                index = i;
                break;
            }
        }

        // Minimum und Maximum Datum der Wetten anhand dem zugewiesenem Event
        ui->tedtWettenErstellenStart->setMinimumDateTime(p_eventListe->holen(index)->getStart());
        ui->tedtWettenErstellenStart->setMaximumDateTime(p_eventListe->holen(index)->getEnde());
        ui->tedtWettenErstellenEnde->setMaximumDateTime(p_eventListe->holen(index)->getEnde());
    }
}

void FrmMain::on_comboMannschaft1Erstellen_currentIndexChanged(const QString &arg1) // Autor: Christoph Rehm
{
    if (arg1 == "Neue hinzufügen")
    {
        ui->tabMain->setCurrentIndex(3);
        ui->tabEinstellungen->setCurrentIndex(1);
        ui->comboMannschaft1Erstellen->setCurrentIndex(0);
    }
}

void FrmMain::on_comboMannschaft2Erstellen_currentIndexChanged(const QString &arg1) // Autor: Christoph Rehm
{
    if (arg1 == "Neue hinzufügen")
    {
        ui->tabMain->setCurrentIndex(3);
        ui->tabEinstellungen->setCurrentIndex(1);
        ui->comboMannschaft2Erstellen->setCurrentIndex(0);
    }
}

void FrmMain::on_comboEventVerwaltungSort_currentIndexChanged(const QString &arg1) // Autor: Christoph Rehm
{
    ui->lwWettenVerwalten->clear();
    ui->btnWettenVerwaltenAktualisieren->setEnabled(false);
    ui->btnWettenVerwaltenLoeschen->setEnabled(false);
    ui->lblWettenVerwaltenEvent->clear();
    ui->lbWettenVerwaltenM1->clear();
    ui->lbWettenVerwaltenM2->clear();
    ui->comboManschaft1Verwaltung->setCurrentIndex(0);
    ui->comboManschaft2Verwaltung->setCurrentIndex(0);

    if (arg1 != "Bitte wählen" && arg1 != "")
    {
        WettenListe *tmpWL = this->p_eventListe->holen(arg1)->getWettenListe();

        for (int i = 0; i < tmpWL->count(); i++)
        {
            int spielID = tmpWL->getSpielIdOf(i);
            ui->lwWettenVerwalten->addItem(tmpWL->getWette(spielID)->getManschaftM1() + " : " + tmpWL->getWette(spielID)->getManschaftM2());
        }
    }
}

void FrmMain::on_btnEinstellungEventsEintragen_clicked() // Autor: Christoph Rehm
{
    QString name = ui->edtEventname->text();
    QString error;
    QDateTime start = ui->tedtEinstellungenEventsStart->dateTime();
    QDateTime ende = ui->tedtEinstellungenEventsEnde->dateTime();
    QString strSiegesPunkte = ui->edtSigesPunkte->text();
    QString strErgebnisPunkte = ui->edtErgebnisPunkte->text();
    int siegesPunkte;
    int ergebnisPunkte;

    if (this->testStringIfInt(strSiegesPunkte) && this->testStringIfInt(strErgebnisPunkte)) // prüfen ob siegesPunkte und ergebnisPunkte auch nur zahlen sind
    {
        siegesPunkte = strSiegesPunkte.toInt();
        ergebnisPunkte = strErgebnisPunkte.toInt();

        if(!p_eventListe->eventAnlegen(name,siegesPunkte,ergebnisPunkte,start,ende,&error)) // event anlegen
        {
            QMessageBox::warning(this,error,error); // wenn ein fehler aufgetreten ist
        }
        else
            this->ausgabeInLwEvents(p_eventListe); // Ausgabe der Events ins ui
    }
    else
        QMessageBox::warning(this,"Falsche eingabe","Falsche eingabe! \nBei der Angabe der Punkte dürfen nur ganze Zahlen eingegeben werden.");



}

void FrmMain::on_btnEinstellungEventsAktualisieren_clicked() // Autor: Christoph Rehm
{
    ui->btnEinstellungEventsAktualisieren->setEnabled(false);
    ui->btnEinstellungEventsLoeschen->setEnabled(false);
    QString name = ui->edtEventname->text();
    QString error;
    int siegesPunkte = ui->edtSigesPunkte->text().toInt();
    int ergebnisPunkte = ui->edtErgebnisPunkte->text().toInt();
    QDateTime start = ui->tedtEinstellungenEventsStart->dateTime();
    QDateTime ende = ui->tedtEinstellungenEventsEnde->dateTime();
    int index = ui->lwEvents->currentRow();

    if (!p_eventListe->eventAktualieren(index,p_eventListe->holen(index)->getName(),name,siegesPunkte,ergebnisPunkte,start,ende,&error))
    {
        QMessageBox::warning(this,error,error);
    }
    else
        this->ausgabeInLwEvents(p_eventListe);
}

void FrmMain::on_btnEinstellungEventsLoeschen_clicked() // Autor: Christoph Rehm
{
    ui->btnEinstellungEventsAktualisieren->setEnabled(false);
    ui->btnEinstellungEventsLoeschen->setEnabled(false);
    int index = ui->lwEvents->currentRow();
    QString name = p_eventListe->holen(index)->getName();
    QString error;

    if (!p_eventListe->eventLoeschen(index,name,&error))
    {
        QMessageBox::warning(this,error,error);
    }
    else
        this->ausgabeInLwEvents(p_eventListe);
}

void FrmMain::on_lwEvents_itemClicked(QListWidgetItem *item) // Autor: Christoph Rehm
{
    ui->btnEinstellungEventsAktualisieren->setEnabled(true);
    ui->btnEinstellungEventsLoeschen->setEnabled(true);
    int index;
    QString name = item->text();

    for (int i = 0; i < p_eventListe->count(); i++)
    {
        if(p_eventListe->holen(i)->getName() == name)
        {
            index = i;
            break;
        }
    }

    ui->edtEventname->setText(name);
    ui->edtErgebnisPunkte->setText(QString::number(p_eventListe->holen(index)->getErgebnisTipp()));
    ui->edtSigesPunkte->setText(QString::number(p_eventListe->holen(index)->getSiegesTipp()));
    ui->tedtEinstellungenEventsStart->setDateTime(p_eventListe->holen(index)->getStart());
    ui->tedtEinstellungenEventsEnde->setDateTime(p_eventListe->holen(index)->getEnde());
}

void FrmMain::on_tedtEinstellungenEventsStart_dateTimeChanged(const QDateTime &dateTime) // Autor: Christoph Rehm
{
    ui->tedtEinstellungenEventsEnde->setMinimumDateTime(dateTime);
}

void FrmMain::on_tedtWettenVerwaltenStart_dateTimeChanged(const QDateTime &dateTime) // Autor: Christoph Rehm
{
    ui->tedtWettenVerwaltenEnde->setMinimumDateTime(dateTime);
}

void FrmMain::on_tedtWettenErstellenStart_dateTimeChanged(const QDateTime &dateTime) // Autor: Christoph Rehm
{
    ui->tedtWettenErstellenEnde->setMinimumDateTime(dateTime);
}

void FrmMain::schreibeLogNachricht(QString nachricht)
{
    QTime qtime = QTime::currentTime();

    QString timeStamp = "[" + QString::number(qtime.hour())
            + ":" + QString::number(qtime.minute()) + "] ";

    this->ui->lwServerLog->addItem(timeStamp + nachricht);
}

void FrmMain::on_btnServerStarten_clicked()
{
    this->ui->btnServerStoppen->setEnabled(true);
    this->ui->btnServerStarten->setEnabled(false);
    QString strPort = ui->edtPort->text();

    if(strPort.isEmpty())
        QMessageBox::information(this,"Information", "Bitte geben Sie einen Port an.");
    else
    {
        if(this->p_serverHandler == NULL)
        {
            this->p_serverHandler = new ServerHandler();
            QObject::connect(this->p_serverHandler, SIGNAL(uebermittleLogNachricht(QString)), this, SLOT(schreibeLogNachricht(QString)));
            this->p_serverHandler->takeAccountListe(this->p_accountListe);
            this->p_serverHandler->takeEventListe(this->p_eventListe);
        }

        this->p_serverHandler->startServer(strPort.toInt());
    }
}

void FrmMain::on_btnServerStoppen_clicked()
{
    this->ui->btnServerStoppen->setEnabled(false);
    this->ui->btnServerStarten->setEnabled(true);

    this->p_serverHandler->stopServer();
}

void FrmMain::on_btnWettenErstellen_clicked() // Autor: Christoph Rehm
{
    QString eventName = ui->comboEventErstellen->currentText();
    QString m1Name = ui->comboMannschaft1Erstellen->currentText();
    QString m2Name = ui->comboMannschaft2Erstellen->currentText();
    QDateTime start = ui->tedtWettenErstellenStart->dateTime();
    QDateTime ende = ui->tedtWettenErstellenEnde->dateTime();

    if(!this->p_eventListe->holen(eventName)->addWette(m1Name,m2Name,start,ende))
        QMessageBox::warning(this,"Fehler","Die Wette konnte nicht erstellt werden! \nEventuell existiert die Wette bereits.");
    else
    {
        ui->comboEventVerwaltungSort->setCurrentIndex(0);
        ui->lwWettenVerwalten->clear();
        QMessageBox::information(this,"Erfolg!", "Wette erfolgreich erstellt!");
    }
}

void FrmMain::on_btnWettenVerwaltenLoeschen_clicked() // Autor: Christoph Rehm
{
    ui->btnWettenVerwaltenAktualisieren->setEnabled(false);
    ui->btnWettenVerwaltenLoeschen->setEnabled(false);
    int indexW = ui->lwWettenVerwalten->currentRow();
    QString nameE = ui->lblWettenVerwaltenEvent->text();
    int spielID = this->p_eventListe->holen(nameE)->getWettenListe()->getSpielIdOf(indexW);

    this->p_eventListe->holen(nameE)->loescheWette(spielID);

    ui->lwWettenVerwalten->clear();
    ui->comboEventVerwaltungSort->setCurrentIndex(0);
    ui->comboEventVerwaltungSort->setCurrentText(nameE);
}

void FrmMain::on_lwWettenVerwalten_itemClicked(QListWidgetItem *item) // Autor: Christoph Rehm
{
    int currentRow = ui->lwWettenVerwalten->currentRow();
    QString nameE = ui->comboEventVerwaltungSort->currentText();

    if (nameE != "Bitte wählen")
    {
        WettenListe* tmpWL = this->p_eventListe->holen(nameE)->getWettenListe();
        int spielID = tmpWL->getSpielIdOf(currentRow);
        Wette* tmpW = tmpWL->getWette(spielID);

        ui->lbWettenVerwaltenM1->setText(tmpW->getManschaftM1());
        ui->lbWettenVerwaltenM2->setText(tmpW->getManschaftM2());
        ui->lblWettenVerwaltenEvent->setText(nameE);
        ui->comboManschaft1Verwaltung->setCurrentText(tmpW->getManschaftM1());
        ui->comboManschaft2Verwaltung->setCurrentText(tmpW->getManschaftM2());
        ui->tedtWettenVerwaltenStart->setDateTime(tmpW->getStart());
        ui->tedtWettenVerwaltenEnde->setDateTime(tmpW->getEnde());
        ui->btnWettenVerwaltenAktualisieren->setEnabled(true);
        ui->btnWettenVerwaltenLoeschen->setEnabled(true);

        if(tmpW->getPunkteM1() != -1)
        {
            ui->edtWettenVerwaltenPunkteM1->setText(QString::number(tmpW->getPunkteM1()));
            ui->edtWettenVerwaltenPunkteM2->setText(QString::number(tmpW->getPunkteM2()));
        }
        else
        {
            ui->edtWettenVerwaltenPunkteM1->clear();
            ui->edtWettenVerwaltenPunkteM2->clear();
        }
    }
}

void FrmMain::on_btnWettenVerwaltenAktualisieren_clicked() // Autor: Christoph Rehm
{
    ui->btnWettenVerwaltenAktualisieren->setEnabled(false);
    ui->btnWettenVerwaltenLoeschen->setEnabled(false);

    int indexW = ui->lwWettenVerwalten->currentRow();
    QString nameE = ui->lblWettenVerwaltenEvent->text();
    int spielID = this->p_eventListe->holen(nameE)->getWettenListe()->getSpielIdOf(indexW);
    QString m1 = ui->comboManschaft1Verwaltung->currentText();
    QString m2 = ui->comboManschaft2Verwaltung->currentText();
    QDateTime start = ui->tedtWettenVerwaltenStart->dateTime();
    QDateTime ende = ui->tedtWettenVerwaltenEnde->dateTime();
    int pM1 = -1;
    int pM2 = -1;

    if(ui->edtWettenVerwaltenPunkteM1->text() != "")
    {
        pM1 = ui->edtWettenVerwaltenPunkteM1->text().toInt();
        pM2 = ui->edtWettenVerwaltenPunkteM2->text().toInt();
    }

    this->p_eventListe->holen(nameE)->updateWette(spielID,m1,m2,start,ende,pM1,pM2);

    ui->lwWettenVerwalten->clear();
    ui->comboEventVerwaltungSort->setCurrentIndex(0);
    ui->comboEventVerwaltungSort->setCurrentText(nameE);
}

void FrmMain::on_tabMain_tabBarClicked(int index) // Autor: Christoph Rehm
{
    if (index == 0)
        this->ladeNowWetten();
    else if(index == 1)
        this->accountsAnzeigen();
}

void FrmMain::ladeNowWetten() // Autor: Christoph Rehm
{
    ui->lwAktuelleWetten->clear();
    this->aktuelleWetten = this->p_eventListe->getNowWetten();

    for (int i = 0; i < aktuelleWetten.size(); i++)
    {
        ui->lwAktuelleWetten->addItem(aktuelleWetten.at(i)->getManschaftM1() + " : " + aktuelleWetten.at(i)->getManschaftM2());
    }
}

void FrmMain::on_lwAktuelleWetten_itemClicked(QListWidgetItem *item) // Autor: Christoph Rehm
{
    int indexW = ui->lwAktuelleWetten->currentRow();

    ui->lblEvent->setText(this->p_eventListe->getEventNameOf(aktuelleWetten.at(indexW)->getSpielID()));
    ui->lblStart->setText(aktuelleWetten.at(indexW)->getStart().toString("yyyy-MM-dd hh:mm:ss"));
    ui->lblEnde->setText(aktuelleWetten.at(indexW)->getEnde().toString("yyyy-MM-dd hh:mm:ss"));

}

void FrmMain::on_btnAendern_clicked() // Autor: Aymane Amaioua
{
    if(this->ui->lwUserVerwalten->currentRow() == -1)
        QMessageBox::information(this, "Information","Es ist kein Nutzer ausgewählt."); // ist kein item gewählt?
    else
    {
        QString currentItemString = this->ui->lwUserVerwalten->currentItem()->text();

        QString benutzername = currentItemString.mid(0,currentItemString.indexOf(" "));
        QString newbenutzername = ui->edtUserBenutzername->text();
        QString vorname = ui->edtUserVorname->text();
        QString nachname = ui->edtUserNachname->text();
        QString klasse = ui->edtUserKlasse->text();
        QString email = ui->edtUserEmail->text();
        QString error = "";
        QString erfolgreich = "Die Daten wurden Erfolgreich geändert :)";

        if(!this->p_accountListe->AccountAendern(benutzername, newbenutzername, vorname, nachname, klasse, email, &error))
            QMessageBox::critical(this,"Fehler!",error);
        else
            QMessageBox::information(this,"Erfolgreich", erfolgreich);

        this->accountsAnzeigen();
    }
}

void FrmMain::on_btnLoeschen_clicked()// Autor: Aymane Amaioua
{
    if(this->ui->lwUserVerwalten->currentRow() == -1)
        QMessageBox::information(this, "Information","Es ist kein Nutzer ausgewählt."); // ist kein item gewählt?
    else
    {
        QString currentItemString = this->ui->lwUserVerwalten->currentItem()->text();

        QString benutzername;
        QString error = "";
        QString erfolgreich = "Der jeweilige Account wurde erfolgreich gelöscht :)";

        if(this->ui->cbUserAnzeigen->currentText() != "Admin")
        {
            benutzername = currentItemString.mid(0,currentItemString.indexOf(" "));

            if(!this->p_accountListe->AccountLoeschen(benutzername, &error))
                QMessageBox::critical(this,"Fehler!",error);
            else
                QMessageBox::information(this,"Erfolgreich", erfolgreich);
        }
        else
        {
            benutzername = currentItemString;
            this->p_accountListe->AdminLoeschen(benutzername,&error);
        }
        this->accountsAnzeigen();
    }
}

void FrmMain::on_checkadmin_clicked()// Autor: Aymane Amaioua
{
    bool ok;
    ok = ui->checkadmin->isChecked();
    if(ok)
    {
        ui->edtEmail->setEnabled(false);
        ui->edtKlasse->setEnabled(false);
        ui->edtVorname->setEnabled(false);
        ui->edtNachname->setEnabled(false);
        ui->edtEmail->clear();
        ui->edtKlasse->clear();
        ui->edtVorname->clear();
        ui->edtNachname->clear();
    }
    else
    {
        ui->edtEmail->setEnabled(true);
        ui->edtKlasse->setEnabled(true);
        ui->edtVorname->setEnabled(true);
        ui->edtNachname->setEnabled(true);
    }
}

void FrmMain::on_cbUserAnzeigen_currentTextChanged(const QString &arg1)
{
    this->accountsAnzeigen();

    if(arg1 == "Admin")
    {
        ui->edtUserBenutzername->setEnabled(false);
        ui->edtUserNachname->setEnabled(false);
        ui->edtUserVorname->setEnabled(false);
        ui->edtUserKlasse->setEnabled(false);
        ui->edtUserEmail->setEnabled(false);
        ui->btnFreischalten->setEnabled(false);
        ui->btnAendern->setEnabled(false);
        ui->lblfrei->hide();
        ui->lblFreigeschaltet->hide();
        ui->lblregseit->hide();
        ui->lbl_reg->hide();
    }
    else
    {
        ui->edtUserBenutzername->setEnabled(true);
        ui->edtUserNachname->setEnabled(true);
        ui->edtUserVorname->setEnabled(true);
        ui->edtUserKlasse->setEnabled(true);
        ui->edtUserEmail->setEnabled(true);
        ui->btnFreischalten->setEnabled(true);
        ui->btnAendern->setEnabled(true);
        ui->lblfrei->show();
        ui->lblFreigeschaltet->show();
        ui->lblregseit->show();
        ui->lbl_reg->show();
    }
}

void FrmMain::accountsAnzeigen() // Autoren: Aymane, Christoph
{
    QString modus = ui->cbUserAnzeigen->currentText();
    ui->lwUserVerwalten->setGeometry(10,40,301,311);

    ui->lwUserVerwalten->clear();

    if(modus == "Alle")
    {
        for(int i = 0; i < this->p_accountListe->getAccounts().length();i++)
        {
            QString user = this->p_accountListe->getAccounts().at(i).getBenutzername()
                   + " (" + this->p_accountListe->getAccounts().at(i).getNachname() + ", " + this->p_accountListe->getAccounts().at(i).getVorname() + ")";

            ui->lwUserVerwalten->addItem(user);
        }
    }
    else if(modus == "Admin")
    {
        for(int i = 0; i < this->p_accountListe->getAdmins().length();i++)
            ui->lwUserVerwalten->addItem(this->p_accountListe->getAdmins().at(i));
    }
    else if(modus == "Freigeschaltet")
    {
        for(int i = 0; i < this->p_accountListe->getAccounts().length();i++)
        {
            if(this->p_accountListe->getAccounts().at(i).getFreigeschaltet())
            {
                QString user = this->p_accountListe->getAccounts().at(i).getBenutzername()
                       + " (" + this->p_accountListe->getAccounts().at(i).getNachname() + ", " + this->p_accountListe->getAccounts().at(i).getVorname() + ")";

                ui->lwUserVerwalten->addItem(user);
            }
        }
    }
    else if(modus == "Gesperrt")
    {
        for(int i = 0; i < this->p_accountListe->getAccounts().length();i++)
        {
            if(!this->p_accountListe->getAccounts().at(i).getFreigeschaltet())
            {
                QString user = this->p_accountListe->getAccounts().at(i).getBenutzername()
                       + " (" + this->p_accountListe->getAccounts().at(i).getNachname() + ", " + this->p_accountListe->getAccounts().at(i).getVorname() + ")";

                ui->lwUserVerwalten->addItem(user);
            }
        }
    }
    else if(modus == "Bestenliste")
    {
        ui->lwUserVerwalten->setGeometry(10,70,301,281);
    }
}

void FrmMain::on_comboUserVerwaltenEvent_currentIndexChanged(const QString &arg1) // Autor Christoph Rehm
{
    this->ui->lwUserVerwalten->clear();

    if (arg1 != "" && arg1 != "Bitte wählen" && ui->cbUserAnzeigen->currentText() == "Bestenliste")
    {
        QVector<EventSpieler*> tmpQV = this->p_eventListe->holen(arg1)->getBestenliste();

        for (int i = 0; i < tmpQV.size(); i++)
        {
            ui->lwUserVerwalten->addItem(tmpQV.at(i)->getSpieler()->getBenutzername()+ "  Punkte: "+ QString::number(tmpQV.at(i)->getPunkte()));
        }
    }
}

void FrmMain::on_lwUserVerwalten_itemClicked(QListWidgetItem *item)
{
    Account selAccount("","","","","","",false,"");
    QString itemText = item->text();
    QString selectedAccountName = itemText.mid(0, itemText.indexOf(" "));

    for(int i = 0; i < this->p_accountListe->getAccounts().length(); i++)
    {
        if(this->p_accountListe->getAccounts().at(i).getBenutzername() == selectedAccountName)
        {
            selAccount = this->p_accountListe->getAccounts().at(i);
            break;
        }
    }

    ui->edtUserBenutzername->setText(selAccount.getBenutzername());
    ui->edtUserEmail->setText(selAccount.getEmail());
    ui->edtUserKlasse->setText(selAccount.getKlasse());
    ui->edtUserNachname->setText(selAccount.getNachname());
    ui->edtUserVorname->setText(selAccount.getVorname());
    ui->lbl_reg->setText(selAccount.getRegDatum());

    if(selAccount.getFreigeschaltet())
    {
        ui->lblFreigeschaltet->setText("JA");
        ui->btnFreischalten->setText("Sperren");
    }
    else
    {
        ui->lblFreigeschaltet->setText("NEIN");
        ui->btnFreischalten->setText("Freischalten");
    }
}

