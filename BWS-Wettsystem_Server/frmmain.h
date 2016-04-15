#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QWidget>
#include <QMessageBox>
#include <QTime>
#include "dbhandler.h"
#include "login.h"
#include "account.h"
#include "accountliste.h"
#include "eventliste.h"
#include "serverhandler.h"

namespace Ui {
class FrmMain;
}

// !!! Bitte Namen immer eindeutig wählen, Struktur beibehalten, gut lesbar formatieren und kommentieren  !!!

class FrmMain : public QWidget
{
    Q_OBJECT

public:
    explicit FrmMain(QWidget *parent = 0);
    ~FrmMain();

private:
    Ui::FrmMain *ui;

    Login* p_login;
    DBHandler* p_dbHandler;              // einzige original DBHandler-Instanz, bei mehreren bestünde nämlcih die Möglichkeit von 2 gleichzeitigen Zugriffen,
    ServerHandler* p_serverHandler;        //Objekte die DBHandler brauchen werden via Konstruktor der Pointer übergen
    AccountListe* p_accountListe;

    QVector<QString> mannschaften;
    void sortAnQVectorQString(QVector<QString>* vector);
    void ausgabeInLwMannschaften(QVector<QString>* tmp);
    void ausgabeInLwEvents(EventListe* tmp);

    void ladeNowWetten();

    bool testStringIfInt(QString tmpString); // überbrüft ob der String nur zahlen enthält

    EventListe* p_eventListe;
    QVector<Wette*> aktuelleWetten;

    void accountsAnzeigen();

signals:
    void openLogin(bool);

private slots:

    void on_btnLogout_clicked();
    void on_btnRegistrieren_clicked();
    void on_tabUserVerwaltung_tabBarClicked(int index);
    void on_btnFreischalten_clicked();

    //void on_tabWidget_tabBarClicked(int index);

    void on_btnEinstellungManschaftenEintragen_clicked();

    void on_btnEinstellungManschaftenLoeschen_clicked();

    void on_lwMannschaften_itemClicked(QListWidgetItem *item);

    void on_btnEinstellungManschaftenAktualisieren_clicked();

    void on_comboManschaft1Verwaltung_currentIndexChanged(const QString &arg1);

    void on_comboManschaft2Verwaltung_currentIndexChanged(const QString &arg1);

    void on_comboEventErstellen_currentIndexChanged(const QString &arg1);

    void on_comboEventVerwaltungSort_currentIndexChanged(const QString &arg1);

    void on_comboMannschaft1Erstellen_currentIndexChanged(const QString &arg1);

    void on_comboMannschaft2Erstellen_currentIndexChanged(const QString &arg1);

    void on_btnEinstellungEventsEintragen_clicked();

    void on_btnEinstellungEventsAktualisieren_clicked();

    void on_btnEinstellungEventsLoeschen_clicked();

    void on_lwEvents_itemClicked(QListWidgetItem *item);

    void on_tedtEinstellungenEventsStart_dateTimeChanged(const QDateTime &dateTime);

    void on_tedtWettenVerwaltenStart_dateTimeChanged(const QDateTime &dateTime);

    void on_tedtWettenErstellenStart_dateTimeChanged(const QDateTime &dateTime);

    void on_btnServerStarten_clicked();

    void on_btnServerStoppen_clicked();

    void showWindow(bool mode);

    void schreibeLogNachricht(QString nachricht);


    void on_btnWettenErstellen_clicked();
    void on_btnWettenVerwaltenLoeschen_clicked();
    void on_lwWettenVerwalten_itemClicked(QListWidgetItem *item);
    void on_btnWettenVerwaltenAktualisieren_clicked();
    void on_tabMain_tabBarClicked(int index);
    void on_lwAktuelleWetten_itemClicked(QListWidgetItem *item);
    void on_btnAendern_clicked();
    void on_btnLoeschen_clicked();
    void on_checkadmin_clicked();
    void on_cbUserAnzeigen_currentTextChanged(const QString &arg1);
    void on_comboUserVerwaltenEvent_currentIndexChanged(const QString &arg1);
    void on_lwUserVerwalten_itemClicked(QListWidgetItem *item);
};

#endif // FRMMAIN_H
