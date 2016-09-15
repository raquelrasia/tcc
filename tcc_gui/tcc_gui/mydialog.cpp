#include "mydialog.h"
#include "ui_mydialog.h"

/************* AUX METHOD *************/
// returns state for current dialog.
bool MyDialog::get_dialog_ended() {
    return dialog_ended ;
}

/************* AUX METHOD *************/
// returns
QString MyDialog::get_rec_dir() {
    return rec_dir ;
}

/************* AUX METHOD *************/
// returns the file name
QString MyDialog::get_filename() {
    return filename ;
}


/************* CONSTRUCTOR METHOD *************/
MyDialog::MyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyDialog)
{
    ui->setupUi(this);

    // Set title
    this->setWindowTitle("Choose Lecture Directory");

    // Set path to standard location (e.g., Videos, Documents, etc)
    dir_videos = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation) + QString("/") ;
    rec_dir = dir_videos ;
    ui->lineEdit->setText(dir_videos);

    // Gets current date
    date = QDate::currentDate().toString("dd_MM_yyyy");

    // The new file will be named after the current date,
    // e.g., 27_08_2016.mkv
    filename = date ;

    // File extension
    file_extension = ".mkv" ;
    full_path = rec_dir + filename + file_extension ;

    // If there's not a file with the same name on the chosen folder,
    // then change button to "clickable", i.e., not grayed out
    if (!QFileInfo(full_path).exists()) {
        ui->pushButton_2->setEnabled(true) ;
        ui->label_8->setVisible(false);
    }

    // If there's a file with the same name, warns user to choose a
    //different path
    else {
        ui->label_8->setVisible(true);
        ui->label_8->setStyleSheet("color: red");
        ui->label_8->setText("File already exists. Please choose a different file name.");
        ui->pushButton_2->setEnabled(false) ;
    }

    dialog_ended = false ;
}

/************* DESTRUCTOR METHOD *************/
MyDialog::~MyDialog()
{
    delete ui;
}


/************* PUSH_BUTTON *************/
// Defines behaviour of the "BROWSE" push button
void MyDialog::on_pushButton_clicked()
{
    // Get directory from browsing dialog
    QString dir_name = QFileDialog::getExistingDirectory(this, tr("Open Directory"), rec_dir,
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks) ;
    // Checks whether dir_name is not empty
    if (dir_name != "") {
        ui->lineEdit->setText(dir_name) ;
        rec_dir = dir_name + QString("/");
    }

    // Concatenates full_path
    full_path = rec_dir + filename + file_extension ;

    // If there's not a file with the same name on the chosen folder,
    // then change button to "clickable", i.e., not grayed out
    if (!QFileInfo(full_path).exists()) {
        ui->pushButton_2->setEnabled(true) ;
        ui->label_8->setVisible(false);
    }

    // If there's a file with the same name, warns user to choose a
    //different path
    else {
        ui->label_8->setVisible(true);
        ui->label_8->setStyleSheet("color: red");
        ui->label_8->setText("File already exists. Please choose a different file name.");
        ui->pushButton_2->setEnabled(false) ;
    }
}


/************* PUSH_BUTTON *************/
// Defines behaviour of the "NEXT" push button
void MyDialog::on_pushButton_2_clicked()
{
    QDir dir(rec_dir) ;

    // Creates a new folder for the recordings
    // This folder's name is the current date
    if (dir.mkpath(rec_dir + date)) {
        rec_dir = rec_dir + date + "/" ;
    }

    // Updates dialog_ended status flag
    // This flag is used to check when the
    // program has successfully reached this point
    dialog_ended = close() ;

    qDebug() << rec_dir ;
}


/************* PUSH_BUTTON *************/
// Defines behaviour of the "CANCEL" push button
void MyDialog::on_pushButton_3_clicked()
{
    close() ;
}
