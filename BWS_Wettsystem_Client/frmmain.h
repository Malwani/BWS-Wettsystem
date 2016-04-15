#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QWidget>
#include <QMessageBox>
#include <QListWidgetItem>

#include "login.h"
#include "register.h"
#include "verbindungsaufbau.h"
#include "clienthandler.h"

namespace Ui {
class FrmMain;
}

class FrmMain : public QWidget
{
    Q_OBJECT

public:
    explicit FrmMain(QWidget *parent = 0);
    ~FrmMain();

private:
    Ui::FrmMain *ui;

    Login login;
    Register reg;
    Verbindungsaufbau verbAufbau;
    ClientHandler clientHandler;

    QVector<EventInfo> events;
    QVector<SpielInfo> aktuelleWetten;
    QVector<SpielerInfo> aktBestenListe;

signals:

    void openLogin(bool);
    void openRegister(bool);
    void openVerbindungsaufbau(bool);

public slots:

    void userLoggedIn(QVector<EventInfo> events);
    void userLoggedOut();
    void verbindungGetrennt();
    void displayNewMail(QString newMail);

    void uebernehmeMeineTipps(QVector<TipInfo> myTips);
    void uebernehmeAktuelleWetten(QVector<SpielInfo> aktWetten);
    void uebernehmeAusgWette(QString start, QString ende, int punkteA, int punkteB, int tipEins, int tipZweiA, int tipZweiB);
    void uebernehmeBestenListe(QVector<SpielerInfo> spieler);
    void uebernehmeAusgSpieler(QString vorname, QString nachname, QString klasse, QString regSeit);

    void tippbereichAusschalten();

    void showWindow(bool mode);

private slots:

    void on_btnLogout_clicked();
    void on_btnMailAendern_clicked();
    void on_btnPassAendern_clicked();
    void on_btnAccountLoeschen_clicked(); // bis hier
    void on_cbEventUebersicht_currentIndexChanged(int index);
    void on_cbEventAktuelleSpiele_currentIndexChanged(int index);
    void on_cbEventBestenliste_currentIndexChanged(int index);
    void on_btnAktSpieleTipAbgeben_clicked();
    void on_lwAktSpiele_itemClicked(QListWidgetItem *item);
    void on_lwBestenliste_itemClicked(QListWidgetItem *item);
    void on_rbSiegestipp_clicked();
    void on_rbErgebnistipp_clicked();
};

#endif // FRMMAIN_H
