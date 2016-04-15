#include <QApplication>
#include "frmmain.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FrmMain frmMain;

    frmMain.openVerbindungsaufbau(true);




    return a.exec();
}
