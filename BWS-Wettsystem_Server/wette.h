#ifndef WETTE_H
#define WETTE_H

#include "tippliste.h"

class Wette
{
public:
    Wette(AccountListe* accountListe, QString m1, QString m2, QDateTime start, QDateTime ende);
    ~Wette();

    void spielTippListeLaden(DBHandler* p_dbHandler);
    void setSpielID(int spielID); // Muss gesetzt werden um andere methoden benutzen zu können

    void setManschaften(QString m1, QString m2);
    void setErgebnis(int punkteM1,int punkteM2);
    void setZeit(QDateTime start, QDateTime ende);

    int getPunkteM1();
    int getPunkteM2();

    QString getManschaftM1();
    QString getManschaftM2();

    QDateTime getStart();
    QDateTime getEnde();

    int getSpielID();

    bool getStatusTippbar();
    bool hatgeTippt(QString benutzername);
    Tipp* getTipp(QString benutzername);

    TippListe* getTippListe();
    void tippListeLoesche(DBHandler *p_dbHandler);

private:
    TippListe* tippListe;
    int spielID;
    QString ManschaftM1;
    QString ManschaftM2;
    int punkteM1;
    int punkteM2;
    QDateTime start;
    QDateTime ende;
};

#endif // WETTE_H
