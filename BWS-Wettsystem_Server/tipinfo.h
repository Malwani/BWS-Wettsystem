#ifndef TIPINFO_H
#define TIPINFO_H

#include <QString>

class TipInfo
{
public:
    TipInfo();

    QString getMannschaftA() const;
    QString getMannschaftB() const;
    QString getDatum() const;
    int getTipEins() const;
    int getTipZweiA() const;
    int getTipZweiB() const;
    bool getSpielStatus() const;

    void setTipInfo(QString mannschaftA, QString mannschaftB, QString datum, bool spielStatus, int tipEins, int tipZweiA, int tipZweiB);
    // int gewonnen = -1 -> user hat nicht gewonnen int gewonnen = 1 -> user hat bei TIp eins richtig Getippt, 2 -> userhat bei tip 2 richtig getippt (Client intern dann preis vom entsprechenden Event hiolen uns ausgeben)

private:

    QString mannschaftA;
    QString mannschaftB;
    QString datum;
    bool spielStatus;
    int tipEins;
    int tipZweiA;
    int tipZweiB;
};

#endif // TIPINFO_H
