#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include "maindialog.h"

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
private slots:
    void on_pushButton_clicked();

private:
    Ui::LoginDialog *ui;

    int status ;
};

#endif // LOGINDIALOG_H
