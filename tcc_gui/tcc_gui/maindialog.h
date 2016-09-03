#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include "mydialog.h"
#include "mycoursedialog.h"
#include "logindialog.h"

#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QProcess>
#include <QFileInfo>
#include <QDate>
#include <QMessageBox>

namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();
    bool fileExists(QString path) ;

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

private:
    Ui::MainDialog *ui;
    QProcess * ffmpeg_process ;
    QProcess * ffmpeg_cat ;

    QString path ;
    QString filename ;
    QString video_file_extension ;
    QString full_path ;
    QString course_code ;
    QString class_code ;
    QString fps ; // Frames per second

    uint8_t number_of_recordings ;

    QFont font ;

    bool dialog_ended ;
    bool autopath_dialog_ended ;
    bool lecture_init ;
    bool recording_init ;
};

#endif // MAINDIALOG_H
