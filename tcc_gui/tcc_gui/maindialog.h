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
#include <QNetworkInterface>
#include <QMutex>
#include <QCloseEvent>

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
    void set_xml_path( QString path);
    void set_username( QString usrname);
    void closeEvent(QCloseEvent *event);
    bool write_remote_xml_cmd_file(QString video_name, QString audio_name);
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_finishedTransfer(int exitCode, QProcess::ExitStatus exitStatus);
private:
    Ui::MainDialog *ui;
    QProcess * ffmpeg_process ;
    //QProcess * ffmpeg_cat ;
    //QProcess * netcat_mplayer_client ;
    //QProcess * rpi_cam ;
    //QProcess * transfer ;

    // Recording directory
    QString rec_dir ;

    // Full path for video recording including audio and video extensions,
    // e.g., C:/User/someone/Videos/IF66D/s11/2016_2/14_09_16.mkv
    QString full_path ;
    QString dir_videos ;
    QString filename ;
    QString audio_file_extension ;
    QString video_file_extension ;
    QString course_code ;
    QString class_code ;
    QString semester ;
    QString year;
    QString fps ; // Frames per second
    QString xml_path;
    QString program_dir ;
    QString xml_video_file_name;
    QString username;
    uint8_t number_of_recordings ;
    QProcess * transfer_files;
    QFont font ;

    bool dialog_ended ;
    bool autopath_dialog_ended ;
    bool lecture_init ;
    bool recording_init ;
};

#endif // MAINDIALOG_H
