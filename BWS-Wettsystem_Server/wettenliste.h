#ifndef WETTENLISTE_H
#define WETTENLISTE_H

#include "wette.h"

class WettenListe
{
public:
    WettenListe(QString *eventName, AccountListe *accountListe, DBHandler *p_dbHandler);
    ~WettenListe();

    void ladenWetten();

    bool addWette(QString m1, QString m2, QDateTime start, QDateTime ende, int spielID = -1, int punkteM1 = -1, int punkteM2 = -1, bool datenbankEintrag = true);

    bool updateWette(int spielID, QString m1, QString m2, QDateTime start, QDateTime ende, int punkteM1 = -1, int punkteM2 = -1);

    bool loescheWette(int spielID);

    bool exist(QString m1, QString m2, QDateTime start, QDateTime ende);

    Wette* getWette(int spielID);
    QVector<Tipp*> getTipps(QString benutzername);
    QVector<Wette*> getSpielerWetten(QString benutzername);
    QVector<Wette*> getWetten();

    int getIndexOf(int spielID);
    int getSpielIdOf(int indexW);
    int getSpielIdOf(QString m1, QString m2, QDateTime start, QDateTime ende);

    int count();

private:
    QString* eventName;
    DBHandler* p_dbHandler;
    AccountListe *accountListe;
    QVector<Wette*> wettenListe;
};

#endif // WETTENLISTE_H
