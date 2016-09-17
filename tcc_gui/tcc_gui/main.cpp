#include "logindialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    LoginDialog login;
    return app.exec();
}
