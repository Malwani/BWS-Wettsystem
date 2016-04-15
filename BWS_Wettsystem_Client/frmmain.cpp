#include "frmmain.h"
#include "ui_frmmain.h"

FrmMain::FrmMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmMain)
{
    ui->setupUi(this);

    /* Signals & Slots Fenstermanagement */

    // FrmMain
    QObject::connect(this, SIGNAL(openLogin(bool))            , &login,      SLOT(showWindow(bool)) );
    QObject::connect(this, SIGNAL(openRegister(bool))         , &reg,        SLOT(showWindow(bool)) );
    QObject::connect(this, SIGNAL(openVerbindungsaufbau(bool)), &verbAufbau, SLOT(showWindow(bool)) );
    // Login
    QObject::connect(&login, SIGNAL(openFrmMain(bool))          , this       , SLOT(showWindow(bool)) );
    QObject::connect(&login, SIGNAL(openRegister(bool))         , &reg       , SLOT(showWindow(bool)) );
    QObject::connect(&login, SIGNAL(openVerbindungsaufbau(bool)), &verbAufbau, SLOT(showWindow(bool)) );
    // Register
    QObject::connect(&reg, SIGNAL(openFrmMain(bool))          , this         , SLOT(showWindow(bool)) );
    QObject::connect(&reg, SIGNAL(openLogin(bool))            , &login       , SLOT(showWindow(bool)) );
    QObject::connect(&reg, SIGNAL(openVerbindungsaufbau(bool)), &verbAufbau  , SLOT(showWindow(bool)) );
    //Verbindungsaufbau
    QObject::connect(&verbAufbau, SIGNAL(openFrmMain(bool)) , this  , SLOT(showWindow(bool)) );
    QObject::connect(&verbAufbau, SIGNAL(openLogin(bool))   , &login, SLOT(showWindow(bool)) );
    QObject::connect(&verbAufbau, SIGNAL(openRegister(bool)), &reg  , SLOT(showWindow(bool)) );
    // ClientHandler
    QObject::connect(&this->clientHandler, SIGNAL(login(QVector<EventInfo>)), this, SLOT(userLoggedIn(QVector<EventInfo>)));
    QObject::connect(&this->clientHandler, SIGNAL(logout()), this, SLOT(userLoggedOut()));
    QObject::connect(&this->clientHandler, SIGNAL(verbindungAbgebrochen()), this, SLOT(verbindungGetrennt()));
    QObject::connect(&this->clientHandler, SIGNAL(displayNewMail(QString)), this, SLOT(displayNewMail(QString)));
    QObject::connect(&this->clientHandler, SIGNAL(registerSucced()), &this->reg, SLOT(registerScucced()));

    QObject::connect(&this->clientHandler, SIGNAL(uebergebeMeineTipps(QVector<TipInfo>)), this, SLOT(uebernehmeMeineTipps(QVector<TipInfo>)));
    QObject::connect(&this->clientHandler, SIGNAL(uebergebeAktuelleWetten(QVector<SpielInfo>)), this, SLOT(uebernehmeAktuelleWetten(QVector<SpielInfo>)));
    QObject::connect(&this->clientHandler, SIGNAL(uebergebeAusgWette(QString,QString,int,int,int,int,int)), this, SLOT(uebernehmeAusgWette(QString,QString,int,int,int,int,int)));
    QObject::connect(&this->clientHandler, SIGNAL(uebergebeBestenliste(QVector<SpielerInfo>)), this, SLOT(uebernehmeBestenListe(QVector<SpielerInfo>)));
    QObject::connect(&this->clientHandler, SIGNAL(uebergebeAusgSpieler(QString,QString,QString,QString)), this, SLOT(uebernehmeAusgSpieler(QString,QString,QString,QString)));
    QObject::connect(&this->clientHandler, SIGNAL(deaktiviereTippBereich()), this, SLOT(tippbereichAusschalten()));

    this->verbAufbau.takeClientHandler(&this->clientHandler); // clientHandler adresse übergeben
    this->login.takeClientHandler(&this->clientHandler);
    this->reg.takeClientHandler(&this->clientHandler);

    this->setFixedSize(this->width(),this->height());
    this->ui->tabHauptReiter->setCurrentIndex(0);

}

FrmMain::~FrmMain()
{
    delete ui;
}

void FrmMain::showWindow(bool mode)
{
    this->setVisible(mode);
}

void FrmMain::displayNewMail(QString newMail)
{
    this->ui->lblMail->setText(newMail);
}

void FrmMain::on_btnLogout_clicked()
{
    // Lokalen User inaktiv setzen
    this->clientHandler.activeUser.setInactive();
    // Logout senden
    this->clientHandler.sendData(this->clientHandler.protokoll.packCR_LOGOUT());
}

void FrmMain::tippbereichAusschalten()
{
    this->ui->btnAktSpieleTipAbgeben->setEnabled(false);

    this->ui->edtAktSpieleTipTeamA->setEnabled(false);
    this->ui->edtAktSpieleTipTeamB->setEnabled(false);
    this->ui->rbAktSpieleTeamAGewinnt->setEnabled(false);
    this->ui->rbAktSpieleTeamBGewinnt->setEnabled(false);
    this->ui->rbAktSpieleUnentschieden->setEnabled(false);

    this->ui->rbErgebnistipp->setEnabled(false);
    this->ui->rbSiegestipp->setEnabled(false);
}

void FrmMain::uebernehmeAusgSpieler(QString vorname, QString nachname, QString klasse, QString regSeit)
{
    int index = this->ui->lwBestenliste->currentRow();

    this->ui->lblAuswRegSeit->setText(regSeit);
    this->ui->lblAuswVorname->setText(vorname);
    this->ui->lblAuswNachname->setText(nachname);
    this->ui->lblAuswKlasse->setText(klasse);

    this->ui->lblAuswPunkte->setText(QString::number(this->aktBestenListe.at(index).getPunkte()));
    this->ui->lblAuswPlatz->setText(QString::number(index + 1));
}

void FrmMain::uebernehmeBestenListe(QVector<SpielerInfo> spieler)
{
    QString out;

    this->aktBestenListe = spieler;
    this->ui->lwBestenliste->clear();

    for(int i = 0; i < spieler.length(); i++)
    {
        out = spieler.at(i).getSpielerName() + " - (" + QString::number(spieler.at(i).getPunkte()) + ")";
        this->ui->lwBestenliste->addItem(out);

        if(spieler.at(i).getSpielerName() == this->clientHandler.activeUser.getActiveUsername())
        {
            this->ui->lblBestMyPunkte->setText(QString::number(spieler.at(i).getPunkte()));
            this->ui->lblBestMyPlatz->setText(QString::number(i + 1));
        }
    }
}

void FrmMain::uebernehmeAusgWette(QString start, QString ende, int punkteA, int punkteB, int tipEins, int tipZweiA, int tipZweiB)
{
    int lwIndex = this->ui->lwAktSpiele->currentRow();

    QString teamA = this->aktuelleWetten.at(lwIndex).getMannschaftA();
    QString teamB = this->aktuelleWetten.at(lwIndex).getMannschaftB();

    this->ui->lblAktSpieleEndeZeit->setText(ende);
    this->ui->lblAktSpieleStartZeit->setText(start);

    this->ui->lblAktSpieleTeamA->setText(teamA);
    this->ui->lblAktSpieleTeamB->setText(teamB);

    this->ui->lblAktSpieleTipp2A->setText(teamA);
    this->ui->lblAktSpieleTipp2B->setText(teamB);
    this->ui->rbAktSpieleTeamAGewinnt->setText(teamA);
    this->ui->rbAktSpieleTeamBGewinnt->setText(teamB);

    if(punkteA == -1)
        this->ui->lblAktSpielePunkteA->setText("/");
    else
        this->ui->lblAktSpielePunkteA->setText(QString::number(punkteA));

    if(punkteA == -1)
        this->ui->lblAktSpielePunkteB->setText("/");
    else
        this->ui->lblAktSpielePunkteB->setText(QString::number(punkteB));

    if(tipEins != -1 || (tipZweiA != -1 && tipZweiB != -1) ) // wurde getippt?
    {
        // gesetzten Tip einfügen und Tippbereich disablen

        this->tippbereichAusschalten(); // erst einma alles ausschalten - später entsprechende anschalten

        if(tipEins != -1) // Wurde auf Sieg getippt? Dann Punkte line edits leeren und tip anzeigem
        {
            this->ui->rbSiegestipp->setChecked(true);
            this->ui->rbErgebnistipp->setChecked(false);

            this->ui->edtAktSpieleTipTeamA->clear();
            this->ui->edtAktSpieleTipTeamB->clear();

            switch(tipEins)
            {
                case 0: // unentschieden getippt
                {
                    this->ui->rbAktSpieleUnentschieden->setChecked(true);
                    this->ui->rbAktSpieleTeamAGewinnt->setChecked(false);
                    this->ui->rbAktSpieleTeamBGewinnt->setChecked(false);
                    break;
                }
                case 1: // team A
                {
                    this->ui->rbAktSpieleUnentschieden->setChecked(false);
                    this->ui->rbAktSpieleTeamAGewinnt->setChecked(true);
                    this->ui->rbAktSpieleTeamBGewinnt->setChecked(false);
                    break;
                }
                case 2: // team b
                {
                    this->ui->rbAktSpieleUnentschieden->setChecked(false);
                    this->ui->rbAktSpieleTeamAGewinnt->setChecked(false);
                    this->ui->rbAktSpieleTeamBGewinnt->setChecked(true);
                    break;
                }
                default: QMessageBox::critical(this,"Fehler!", "SR_SG ungültig."); break;
            }     
        }
        else if(tipZweiA != -1 && tipZweiB != -1) // wurde auf Punkt gesetzt? dann alle CB checks raus nehmen und punkte anzeigen
        {
            if(!this->aktuelleWetten.at(lwIndex).getSpielStatus())
            {
                this->ui->rbErgebnistipp->setEnabled(false);
                this->ui->rbSiegestipp->setEnabled(false);
            }

            this->ui->rbSiegestipp->setChecked(false);
            this->ui->rbErgebnistipp->setChecked(true);

            this->ui->rbAktSpieleUnentschieden->setChecked(false);
            this->ui->rbAktSpieleTeamAGewinnt->setChecked(false);
            this->ui->rbAktSpieleTeamBGewinnt->setChecked(false);

            this->ui->edtAktSpieleTipTeamA->setText(QString::number(tipZweiA));
            this->ui->edtAktSpieleTipTeamB->setText(QString::number(tipZweiB));
        }
        else
            QMessageBox::critical(this,"Fehler!", "SR_SG ungültig!");
    }
    else if(tipEins == -1 || (tipZweiA == -1 && tipZweiB == -1) ) // wurde nicht getippt?
    {
        this->ui->edtAktSpieleTipTeamA->clear(); // Eingabe clearen
        this->ui->edtAktSpieleTipTeamB->clear();
        this->ui->rbAktSpieleTeamAGewinnt->setChecked(false);
        this->ui->rbAktSpieleTeamBGewinnt->setChecked(false);
        this->ui->rbAktSpieleUnentschieden->setChecked(false);

        if(this->aktuelleWetten.at(lwIndex).getSpielStatus()) // wenn wette noch läuft
        {
            this->ui->rbErgebnistipp->setEnabled(true);
            this->ui->rbSiegestipp->setEnabled(true);
            this->ui->rbErgebnistipp->setChecked(false);
            this->ui->rbSiegestipp->setChecked(false);
        }
    }
}

void FrmMain::uebernehmeAktuelleWetten(QVector<SpielInfo> aktWetten)
{
    int eventIndex = this->ui->cbEventAktuelleSpiele->currentIndex() - 1;

    this->aktuelleWetten = aktWetten;

    this->ui->lwAktSpiele->clear();
    this->ui->lblAktSpieleErgebnisPunkte->setText(QString("(") + QString::number(this->events.at(eventIndex).getPreisTipEins()) + "P)");
    this->ui->lblAktSpieleSiegesPunkte->setText(QString("(") + QString::number(this->events.at(eventIndex).getPreisTipZwei()) + "P)");

    QString out;

    for(int i = 0; i < aktWetten.length(); i++)
    {
        out = this->aktuelleWetten.at(i).getMannschaftA() +  " vs. " + this->aktuelleWetten.at(i).getMannschaftB() + "\n" + this->aktuelleWetten.at(i).getDatum() + "\nStatus: ";

        if(this->aktuelleWetten.at(i).getSpielStatus())
            out += "Wette läuft.";
        else
            out += "Wette abgelaufen.";

        out += "\n------------------------";

        this->ui->lwAktSpiele->addItem(out);
    }
}

void FrmMain::uebernehmeMeineTipps(QVector<TipInfo> myTips)
{    
    QString out;

    this->ui->lwMeineTips->clear();

    for(int i = 0; i < myTips.length(); i++)
    {
        int tipEins = myTips.at(i).getTipEins();
        int tipZweiA = myTips.at(i).getTipZweiA(), tipZweiB = myTips.at(i).getTipZweiB();

        out = myTips.at(i).getMannschaftA() + " vs. " + myTips.at(i).getMannschaftB() + "\n" + myTips.at(i).getDatum() + "\n\n";

        if(tipEins == -1) // Wenn auf Ergebnis getippt
            out += "Getippt auf Ergebnis - " + QString::number(myTips.at(i).getTipZweiA()) + " : " + QString::number(myTips.at(i).getTipZweiB());
        else if(tipZweiA == -1 && tipZweiB == -1) // Wenn auf Sieg / niederlage/unentschieden getippt
        {
            switch(tipEins)
            {
            case 0: out += "Tipp auf unentschieden."; break;
            case 1: out += "Tipp auf Team [" + myTips.at(i).getMannschaftA() + "] gewinnt."; break;
            case 2: out += "Tipp auf Team [" + myTips.at(i).getMannschaftB() + "] gewinnt."; break;
            default: QMessageBox::critical(this,"Fehler", "Eingegangene Tippdaten ungültig.");
            }
        }

        out += "\nStatus: ";

        if(myTips.at(i).getSpielStatus())
            out += "Wette läuft noch";
        else
            out += "Wette beendet.";

        out += "\n------------------------";

        this->ui->lwMeineTips->addItem(out);
    }

    this->ui->lblLaufendeTips->setText(QString::number(this->ui->lwMeineTips->count()));
}

void FrmMain::userLoggedIn(QVector<EventInfo> events)
{
    emit this->openLogin(false);
    emit this->openRegister(false);

    // Accountdaten ausgeben
    this->ui->lblBenutzernameProfil->setText(this->clientHandler.activeUser.getActiveUsername());
    this->ui->lblCurrentUser->setText(this->clientHandler.activeUser.getActiveUsername());
    this->ui->lblBenutzernameUebersicht->setText(this->clientHandler.activeUser.getActiveUsername());
    this->ui->lblVorname->setText(this->clientHandler.activeUser.getVorname());
    this->ui->lblNachname->setText(this->clientHandler.activeUser.getNachname());
    this->ui->lblKlasse->setText(this->clientHandler.activeUser.getKlasse());
    this->ui->lblMail->setText(this->clientHandler.activeUser.getEmail());
    this->ui->lblRegSeit->setText(this->clientHandler.activeUser.getRegSeit());

    //Events einlesen
    this->events = events;

    this->ui->cbEventAktuelleSpiele->clear();
    this->ui->cbEventBestenliste->clear();
    this->ui->cbEventUebersicht->clear();

    this->ui->cbEventAktuelleSpiele->addItem("Bitte wählen");
    this->ui->cbEventBestenliste->addItem("Bitte wählen");
    this->ui->cbEventUebersicht->addItem("Bitte wählen");

    for(int i = 0; i < events.length(); i++)
    {
        this->ui->cbEventAktuelleSpiele->addItem(events.at(i).getEventName());
        this->ui->cbEventBestenliste->addItem(events.at(i).getEventName());
        this->ui->cbEventUebersicht->addItem(events.at(i).getEventName());
    }
    this->show();
}

void FrmMain::verbindungGetrennt()
{
    // user inaktiv setzen und alle Fenster außer Vebr aufbau schließen
    this->clientHandler.activeUser.setInactive();
    this->hide();
    this->openLogin(false);
    this->openRegister(false);
    this->openVerbindungsaufbau(true);

    QMessageBox::information(this, "Information", "Verbindung vom Server wurde getrennt.");
}

void FrmMain::userLoggedOut()
{
    emit this->hide();
    emit this->openLogin(true);
}

void FrmMain::on_btnMailAendern_clicked()
{
    if(this->ui->edtAccountSektionAktuellesPasswort->text().isEmpty())
        QMessageBox::information(this,"Inforamtion", "Bitte aktuelles Passwort eingeben.");
    else
    {
        QString userpass(QCryptographicHash::hash(ui->edtAccountSektionAktuellesPasswort->text().toUtf8(),QCryptographicHash::Md5).toHex());
        this->clientHandler.setTryMail(this->ui->edtAccountSektionNeueEmail->text());

        // Mailänderung senden
        this->clientHandler.sendData(this->clientHandler.protokoll.packCR_CMAIL(userpass, this->clientHandler.getTryMail() ));
    }
}

void FrmMain::on_btnPassAendern_clicked()
{
    if(this->ui->edtAccountSektionNeuesPasswort->text() != this->ui->edtAccountSektionNeuesPasswortWiederholt->text())
        QMessageBox::information(this, "Information", "Neues Passwort stimmt nicht überein");
    else
    {
        if(this->ui->edtAccountSektionAktuellesPasswort->text().isEmpty())
            QMessageBox::information(this,"Information", "Bitte aktuelles Passwort eingeben.");
        else
        {
            QString userpass(QCryptographicHash::hash(ui->edtAccountSektionAktuellesPasswort->text().toUtf8(),QCryptographicHash::Md5).toHex());
            this->clientHandler.sendData(this->clientHandler.protokoll.packCR_CPASS(userpass, QString(QCryptographicHash::hash(ui->edtAccountSektionNeuesPasswort->text().toUtf8(),QCryptographicHash::Md5).toHex())));
        }
    }
}

void FrmMain::on_btnAccountLoeschen_clicked()
{
    if(this->ui->edtAccountSektionAktuellesPasswort->text().isEmpty())
        QMessageBox::information(this, "Information", "Bitte gebe dein aktuelles Passwort ein.");
    else
    {
        QString userpass(QCryptographicHash::hash(ui->edtAccountSektionAktuellesPasswort->text().toUtf8(),QCryptographicHash::Md5).toHex());

        this->clientHandler.sendData(this->clientHandler.protokoll.packCR_DACC(userpass));
    }
}

void FrmMain::on_cbEventUebersicht_currentIndexChanged(int index)
{
    if(index > 0) // nicht auf Bitte wählen
    {
        QString eventName = this->ui->cbEventUebersicht->currentText();
        QString paket = this->clientHandler.protokoll.packCR_UO(eventName);
        this->clientHandler.sendData(paket);
    }
    else
        this->ui->lwMeineTips->clear();
}

void FrmMain::on_cbEventBestenliste_currentIndexChanged(int index)
{
    if(index > 0) // nicht auf Bitte wählen
    {
        QString eventName = this->ui->cbEventBestenliste->currentText();
        this->clientHandler.sendData(this->clientHandler.protokoll.packCR_HST(eventName));
    }
    else
        this->ui->lwBestenliste->clear();
}

void FrmMain::on_btnAktSpieleTipAbgeben_clicked()
{
    int spielId = this->aktuelleWetten.at(this->ui->lwAktSpiele->currentRow()).getSpielId();

    if(this->ui->rbErgebnistipp->isChecked())
    {
        this->clientHandler.sendData(this->clientHandler.protokoll.packCR_SUBT(spielId,-1, this->ui->edtAktSpieleTipTeamA->text().toInt(), this->ui->edtAktSpieleTipTeamB->text().toInt()));
    }
    else if(this->ui->rbSiegestipp->isChecked())
    {
        int siegesTip;

        if(ui->rbAktSpieleUnentschieden->isChecked())
            siegesTip = 0;
        else if(ui->rbAktSpieleTeamAGewinnt->isChecked())
            siegesTip = 1;
        else if(ui->rbAktSpieleTeamBGewinnt->isChecked())
            siegesTip = 2;

        this->clientHandler.sendData(this->clientHandler.protokoll.packCR_SUBT(spielId, siegesTip, -1,-1));
    }
    else
        QMessageBox::information(this,"Information", "Gebe erst an, auf was du tippen möchtest.");
}

void FrmMain::on_cbEventAktuelleSpiele_currentIndexChanged(int index)
{
    if(index > 0) // nicht auf Bitte wählen
    {
        QString eventName = this->ui->cbEventAktuelleSpiele->currentText();

        this->ui->lblAktSpieleEndeZeit->clear();
        this->ui->lblAktSpieleStartZeit->clear();
        this->ui->lblAktSpielePunkteA->setText("/");
        this->ui->lblAktSpielePunkteB->setText("/");
        this->ui->rbAktSpieleTeamAGewinnt->setText("Mannschaft A");
        this->ui->rbAktSpieleTeamBGewinnt->setText("Mannschaft B");
        this->ui->lblAktSpieleTeamA->setText("Mannschaft A");
        this->ui->lblAktSpieleTeamB->setText("Mannschaft B");
        this->ui->lblAktSpieleTipp2A->setText("Mannschaft A");
        this->ui->lblAktSpieleTipp2B->setText("Mannschaft B");

        this->clientHandler.sendData(this->clientHandler.protokoll.packCR_CG(eventName));
    }
    else
    {
        this->ui->lwAktSpiele->clear();
        this->ui->lblAktSpieleEndeZeit->clear();
        this->ui->lblAktSpieleStartZeit->clear();
        this->ui->lblAktSpielePunkteA->setText("/");
        this->ui->lblAktSpielePunkteB->setText("/");
        this->ui->rbAktSpieleTeamAGewinnt->setText("Mannschaft A");
        this->ui->rbAktSpieleTeamBGewinnt->setText("Mannschaft B");
        this->ui->lblAktSpieleTeamA->setText("Mannschaft A");
        this->ui->lblAktSpieleTeamB->setText("Mannschaft B");
        this->ui->lblAktSpieleTipp2A->setText("Mannschaft A");
        this->ui->lblAktSpieleTipp2B->setText("Mannschaft B");
        this->ui->lblAktSpieleErgebnisPunkte->setText("");
        this->ui->lblAktSpieleSiegesPunkte->setText("");
        this->tippbereichAusschalten();
    }
}

void FrmMain::on_lwAktSpiele_itemClicked(QListWidgetItem *item)
{
    int index = this->ui->lwAktSpiele->currentRow();

    if(this->ui->lwAktSpiele->count() > 0)
    {
        int spielId = this->aktuelleWetten.at(index).getSpielId();
        this->clientHandler.sendData(this->clientHandler.protokoll.packCR_SG(spielId));
    }
}

void FrmMain::on_lwBestenliste_itemClicked(QListWidgetItem *item)
{
    int index = this->ui->lwBestenliste->currentRow();

    if(this->ui->lwBestenliste->count() > 0)
        this->clientHandler.sendData(this->clientHandler.protokoll.packCR_SELP(this->aktBestenListe.at(index).getSpielerName()));
}

void FrmMain::on_rbSiegestipp_clicked()
{
    this->ui->btnAktSpieleTipAbgeben->setEnabled(true);

    this->ui->rbAktSpieleTeamAGewinnt->setEnabled(true);
    this->ui->rbAktSpieleTeamBGewinnt->setEnabled(true);
    this->ui->rbAktSpieleUnentschieden->setEnabled(true);

    this->ui->rbAktSpieleTeamAGewinnt->setCheckable(true);
    this->ui->rbAktSpieleTeamBGewinnt->setCheckable(true);
    this->ui->rbAktSpieleUnentschieden->setCheckable(true);

    this->ui->edtAktSpieleTipTeamA->setEnabled(false);
    this->ui->edtAktSpieleTipTeamB->setEnabled(false);
}

void FrmMain::on_rbErgebnistipp_clicked()
{
    this->ui->btnAktSpieleTipAbgeben->setEnabled(true);

    this->ui->rbAktSpieleTeamAGewinnt->setEnabled(false);
    this->ui->rbAktSpieleTeamBGewinnt->setEnabled(false);
    this->ui->rbAktSpieleUnentschieden->setEnabled(false);

    this->ui->edtAktSpieleTipTeamA->setEnabled(true);
    this->ui->edtAktSpieleTipTeamB->setEnabled(true);
}
