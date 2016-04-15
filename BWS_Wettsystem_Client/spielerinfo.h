#ifndef SPIELERINFO_H
#define SPIELERINFO_H

#include <QString>


class SpielerInfo
{

private:

    QString spielerName;
    int punkte;

public:

    SpielerInfo();

    QString getSpielerName() const;
    int     getPunkte() const;

    void setSpielerInfo(QString spielerName, int punkte);
};

#endif // SPIELERINFO_H
