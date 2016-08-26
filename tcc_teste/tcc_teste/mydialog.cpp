#include "mydialog.h"
#include "ui_mydialog.h"

bool MyDialog::get_dialog_ended() {
    return dialog_ended ;
}

QString MyDialog::get_path() {
    return path ;
}

QString MyDialog::get_filename() {
    return filename ;
}

QString MyDialog::get_file_type() {
    return file_type ;
}



MyDialog::MyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Choose Lecture Directory");

    path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + QString("/") ;
    ui->lineEdit->setText(path);

    date = QDate::currentDate().toString("dd_MM_yyyy");

    filename = date ;

    file_type = ".mkv" ;
    full_path = path + filename + file_type ;


    if (!QFileInfo(full_path).exists()) {
        ui->pushButton_2->setEnabled(true) ;
        ui->label_8->setVisible(false);
    }
    else {
        ui->label_8->setVisible(true);
        ui->label_8->setStyleSheet("color: red");
        ui->label_8->setText("File already exists. Please choose a different file name.");
        ui->pushButton_2->setEnabled(false) ;
    }

    dialog_ended = false ;
}

MyDialog::~MyDialog()
{
    delete ui;
}

void MyDialog::on_pushButton_clicked() // OK
{
    QString dir_name = QFileDialog::getExistingDirectory(this, tr("Open Directory"), path,
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks) ;
    if (dir_name != "") {
        ui->lineEdit->setText(dir_name) ;
        path = dir_name + QString("/");
    }

    full_path = path + filename + file_type ;

    if (!QFileInfo(full_path).exists()) {
        ui->pushButton_2->setEnabled(true) ;
        ui->label_8->setVisible(false);
    }
    else {
        ui->label_8->setVisible(true);
        ui->label_8->setStyleSheet("color: red");
        ui->label_8->setText("File already exists. Please choose a different file name.");
        ui->pushButton_2->setEnabled(false) ;
    }
}

void MyDialog::on_pushButton_2_clicked()
{
    QDir dir(path) ;

    if (dir.mkpath(path + date)) {
        path = path + date + "/" ;
    }
    dialog_ended = close() ;
}

void MyDialog::on_pushButton_3_clicked()
{
    close() ;
}
