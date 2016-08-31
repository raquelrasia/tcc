#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>
#include <QString>
#include <QFileDialog>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDate>

#include <QDebug>

namespace Ui {
class MyDialog;
}

class MyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MyDialog(QWidget *parent = 0);
    ~MyDialog();
    bool get_dialog_ended() ;
    QString get_path() ;
    QString get_filename() ;
    QString get_file_extension() ;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();


private:
    Ui::MyDialog *ui;

    QString path ;
    QString filename ;
    QString full_path ;
    QString file_extension ;
    QString date ;

    bool dialog_ended ;
};

#endif // MYDIALOG_H
