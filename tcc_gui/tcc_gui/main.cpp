#include "logindialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    LoginDialog login;
    login.show();

    return app.exec();
}