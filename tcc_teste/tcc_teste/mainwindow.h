#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QProcess>
#include <QCloseEvent>
#include <QFileInfo>
#include <QDate>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);
    bool fileExists(QString path) ;
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

    void on_actionSair_triggered();

private:
    Ui::MainWindow *ui;
    QProcess * ffmpeg_process ;
    QProcess * ffmpeg_cat ;
    QString path ;
    QString filename ;
    QString video_file_type ;
    QString full_path ;

    QString fps ; // Frames per second
    uint8_t number_of_recordings ;

    QFont font ;

    bool dialog_ended ;
    bool lecture_init ;
    bool recording_init ;
};

#endif // MAINWINDOW_H
