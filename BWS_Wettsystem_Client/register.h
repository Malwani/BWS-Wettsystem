#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include <QMessageBox>
#include <QCryptographicHash>
#include "clienthandler.h"

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = 0);
    ~Register();

    void takeClientHandler(ClientHandler* p_clientHandler);

private:
    Ui::Register *ui;

    ClientHandler* p_clientHandler;

signals:
    void openFrmMain(bool);
    void openLogin(bool);
    void openVerbindungsaufbau(bool);

public slots:

    void showWindow(bool mode);
    void registerScucced();
private slots:
    void on_btnRegistrieren_clicked();
};

#endif // REGISTER_H
