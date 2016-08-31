#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Login");
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_pushButton_clicked()
{
    QString username = ui->lineEdit->text() ;
    QString password = ui->lineEdit_2->text() ;


    /************ LOGIN DATA ROUTINE HERE ************/
    /************ LOGIN DATA ROUTINE HERE ************/
    /************ LOGIN DATA ROUTINE HERE ************/
    /************ LOGIN DATA ROUTINE HERE ************/

    if(username == "user" && password == "12345") {
        close() ;
        MainDialog mdialog ;
        mdialog.setModal(true);
        mdialog.exec() ;
    }


    else {
        QMessageBox::warning(this, "Login", "Your account of password is incorrect") ;
    }
}


void LoginDialog::closeEvent(QCloseEvent *event) {
    event->accept();
}
