#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include "clienthandler.h"

namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0);
    ~Client();

public slots:

    void takeNewData(QString data);

public slots:

    //void logNachrichtAnnehmen(QString daten);

private slots:
    void on_btnVerbinden_clicked();

    void on_btnSenden_clicked();

private:
    Ui::Client *ui;
    ClientHandler clientHandler;
};

#endif // CLIENT_H
