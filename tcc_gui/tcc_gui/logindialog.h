#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include "maindialog.h"
#include "networkconnection.h"

#include <QDialog>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
    void closeEvent(QCloseEvent *event);
    int get_status() ;
    QString make_xml_path();
    void write_auth_xml();
    bool read_auth_xml(QString * username, int * days_expire);

private slots:
    void on_pushButton_clicked();
    void on_ContinueSession_clicked();

private:
    Ui::LoginDialog *ui;
	QString auth_file_path;
    int status ;
    QString username;
};

#endif // LOGINDIALOG_H
