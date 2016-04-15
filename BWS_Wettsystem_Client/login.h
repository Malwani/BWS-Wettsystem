#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QMessageBox>
#include <QCryptographicHash>
#include "clienthandler.h"

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

    void takeClientHandler(ClientHandler* p_clientHandler);

private:
    Ui::Login *ui;

    ClientHandler* p_clientHandler;

signals:
    void openFrmMain(bool);
    void openRegister(bool);
    void openVerbindungsaufbau(bool);

private slots:
    void on_btnLogin_clicked();

    void on_btnRegistrieren_clicked();

    void on_pushButton_clicked();

public slots:
    void showWindow(bool mode);
};

#endif // LOGIN_H
