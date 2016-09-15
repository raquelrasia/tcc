#include "maindialog.h"
#include "ui_maindialog.h"


/************* AUX METHOD *************/
// Checks whether a path exists
bool check_path(QString path) {
    QFileInfo check_file(path);

    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}


/************* CONSTRUCTOR METHOD *************/
MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);

#ifdef __APPLE__ // If on a Mac system, this corrects a menubar bug
    ui->menuBar->setNativeMenuBar(false) ;
#endif
    // Sets window title
    this->setWindowTitle("Lecture Recorder");

    // Configures NEW LECTURE and END LECTURE push buttons and menubar
    ui->pushButton_3->setEnabled(false);
    ui->checkBox_2->setChecked(true);

    // Configures RECORD and STOP RECORDING push buttons
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_4->setEnabled(false);

    // Configures DOCUMENT CAMERA push button
    ui->pushButton_5->setEnabled(false);

    // Configures STATUS label
    ui->label->setStyleSheet("color: red") ;
    ui->label->setText("Status: NOT RECORDING") ;

    // Apply some font changes
    font = ui->label->font() ;
    font.setPointSize(14);
    ui->label->setFont(font);

    // Initializes some variables
    dialog_ended = false ;
    number_of_recordings = 0 ;
    fps = "1" ;
    audio_file_extension = QString(".mp3") ;
    video_file_extension = QString(".mkv") ;

}


/************* DESTRUCTOR METHOD *************/
MainDialog::~MainDialog()
{
    delete ui;
}


/************* PUSH_BUTTON *************/
// Defines behaviour of the "NEW LECTURE" push button
void MainDialog::on_pushButton_clicked()
{
    if(number_of_recordings != 0) {
        number_of_recordings = 0 ;
    }

    // If automatic path is disabled, the user
    // is prompted to choose the directory on which
    // he/she wants the recording to be saved
    if (!(ui->checkBox_2->isChecked())){
        MyCourseDialog mcDialog ;
        mcDialog.set_dir_mode(false) ;
        mcDialog.setModal(true) ;
        mcDialog.exec() ;

        if(mcDialog.get_dialog_ended()) {
            course_code = mcDialog.get_current_course() ;
            class_code = mcDialog.get_current_class() ;

            autopath_dialog_ended = mcDialog.get_dialog_ended() ;
            ui->pushButton->setEnabled(false);
            ui->pushButton_3->setEnabled(true);
            ui->pushButton_2->setEnabled(true);
            ui->pushButton_5->setEnabled(true);
            ui->checkBox_2->setEnabled(false);

            semester = mcDialog.get_current_semester() ;
            filename = mcDialog.get_filename() ;

            MyDialog mDialog ;
            mDialog.setModal(true) ;
            mDialog.exec() ;

            if(mDialog.get_dialog_ended()) {
                dialog_ended = mDialog.get_dialog_ended() ;
                ui->pushButton->setEnabled(false);
                ui->pushButton_3->setEnabled(true);
                ui->pushButton_2->setEnabled(true);

                rec_dir = mDialog.get_rec_dir() ;
            }
        }
    }

    // If automatic path is enabled, the user
    // chooses the course and the class and the
    // program automatically creates the recording
    // directory
    else {
        MyCourseDialog mcDialog ;
        mcDialog.setModal(true) ;
        mcDialog.exec() ;

        if(mcDialog.get_dialog_ended()) {
            course_code = mcDialog.get_current_course() ;
            class_code = mcDialog.get_current_class() ;

            autopath_dialog_ended = mcDialog.get_dialog_ended() ;
            ui->pushButton->setEnabled(false);
            ui->pushButton_3->setEnabled(true);
            ui->pushButton_2->setEnabled(true);
            ui->pushButton_5->setEnabled(true);
            ui->checkBox_2->setEnabled(false);

            rec_dir = mcDialog.get_rec_dir() ;
            dir_videos = mcDialog.get_dir_videos() ;
            semester = mcDialog.get_current_semester() ;
            filename = mcDialog.get_filename() ;
        }
    }

    qDebug() << course_code << class_code << semester << rec_dir << filename << video_file_extension << audio_file_extension ;
}



/************* PUSH_BUTTON *************/
// Defines behaviour of the "END LECTURE" push button
void MainDialog::on_pushButton_3_clicked()
{

    if(QFileInfo::exists(rec_dir + filename + QString::number(1) + video_file_extension) ) {

        if(ui->checkBox_2->isChecked()) {
            /*********** Creates a .bat file with the transfer routine if autopath is used ***********/
            QString transfer_cmds = QString("C:/Users/luigu/Desktop/tcc_gui/tcc_gui/transfer_test.bat") ;
            QFile bat_file(transfer_cmds);
                if (!bat_file.open(QIODevice::WriteOnly | QIODevice::Text))
                    return;

            QTextStream outcmds(&bat_file);
            outcmds << "ssh pi@169.254.96.87 \"mkdir -p /home/pi/Videos/" << course_code + QString("/") +
                    class_code + QString("/") + semester + QString("/") + "\"\n" +
                    QString("pscp -pw raspberry ") + rec_dir + filename + video_file_extension + QString(" ") +
                    rec_dir + filename + audio_file_extension +
                    QString(" pi@169.254.96.87:Videos/") + course_code + QString("/") +
                    class_code + QString("/") + semester + QString("/")  << "\n" ;
            /**************************************************************************/
        }

        else {
            /*********************************************************************************************/
            /*********************************************************************************************/
            /*********************************************************************************************/
            /*********** Creates a .bat file with the transfer routine if autopath is NOT used ***********/
            QString transfer_cmds = QString("C:/Users/luigu/Desktop/tcc_gui/tcc_gui/transfer_test.bat") ;
            QFile bat_file(transfer_cmds);
                if (!bat_file.open(QIODevice::WriteOnly | QIODevice::Text))
                    return;

            QTextStream outcmds(&bat_file);
            outcmds << "ssh pi@169.254.96.87 \"mkdir -p /home/pi/Videos/" << course_code + QString("/") +
                    class_code + QString("/") + semester + QString("/") + "\"\n" +
                    QString("pscp -pw raspberry ") + rec_dir + filename + video_file_extension + QString(" ") +
                    rec_dir + filename + audio_file_extension +
                    QString(" pi@169.254.96.87:Videos/") + course_code + QString("/") +
                    class_code + QString("/") + semester + QString("/")  << "\n" ;
            /**************************************************************************/
        }

        /*********** Creates a .bat file to be transfered to the Raspberry Pi.  ***********/
        /*********** This .bat file is eesponsible for concatenating audio and video files. ***********/
        QString merge_cmds = QString("C:/Users/luigu/Desktop/tcc_gui/tcc_gui/merge_test.bat") ;
        QFile merge_bat_file(merge_cmds);
            if (!merge_bat_file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;

        QTextStream out_merge_cmds(&merge_bat_file);
        out_merge_cmds << "ffmpeg -i " + filename + video_file_extension + " -i " + filename + audio_file_extension +
                          " -c:v copy -c:a copy " + filename + QString("_av") + video_file_extension << "\n" ;
        /**************************************************************************/

        /*********** Creates a text file with the video files to be concatenated ***********/
        QString video_list = rec_dir + QString("video_list.txt") ;
        QFile filevideo(video_list);
            if (!filevideo.open(QIODevice::WriteOnly | QIODevice::Text))
                return;

        QTextStream outvideo(&filevideo);
        for (uint8_t count = 1; count <= number_of_recordings; count++) {
            outvideo << "file '" << filename + QString::number(count) + video_file_extension << "'\n";
        }
        /****************************************************************************/

        /*********** Creates a text file with the audio files' to be concatenated ***********/
        QString audio_list = rec_dir + QString("audio_list.txt") ;
        QFile fileaudio(audio_list);
            if (!fileaudio.open(QIODevice::WriteOnly | QIODevice::Text))
                return;

        QTextStream outaudio(&fileaudio);
        for (uint8_t count = 1; count <= number_of_recordings; count++) {
            outaudio << "file '" << filename + QString::number(count) + ".mp3" << "'\n";
        }

        /****************************************************************************/

        /*********************** Concatenates audio and video files ************************/
        ffmpeg_cat = new QProcess(this) ;
        QStringList ffmpeg_cat_args ;

                        /******************** Video ********************/
        ffmpeg_cat_args << "-y"
                        << "-f"
                        << "concat"
                        << "-i"
                        << rec_dir + QString("video_list.txt")
                        << "-c"
                        << "copy"
                        << "-an"
                        << rec_dir + filename + video_file_extension

                        /******************** Audio ********************/
                        << "-f"
                        << "concat"
                        << "-i"
                        << rec_dir + QString("audio_list.txt")
                        << "-c"
                        << "copy"
                        << rec_dir + filename + audio_file_extension ;

        //connect(ffmpeg_cat, SIGNAL(finished(int,QProcess::ExitStatus)), ffmpeg_cat, SLOT(deleteLater())) ;
        ffmpeg_cat->startDetached("ffmpeg", ffmpeg_cat_args) ;
        ffmpeg_cat->waitForFinished() ;
        ffmpeg_cat->close();
        /**************************************************************************/
    }

    ui->pushButton->setEnabled(true) ;
    ui->pushButton_2->setEnabled(false) ;
    ui->pushButton_3->setEnabled(false) ;
    ui->pushButton_4->setEnabled(false) ;
    ui->pushButton_5->setEnabled(false) ;
    ui->checkBox_2->setEnabled(true);
    ui->label->setStyleSheet("color: orange");
    ui->label->setText("Status: Lecture Finished");

    transfer = new QProcess(this) ;
    transfer->startDetached("C:/Users/luigu/Desktop/tcc_gui/tcc_gui/transfer_test.bat") ;

    ui->label->setText("Status: Lecture Finished");

    /*********** Displays message asking if user wants to upload video now ***********/
    QMessageBox msgBox ;
    msgBox.setInformativeText("Do you wish to transfer the files now?") ;
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();

    if(ret == QMessageBox::Yes){
        /****** Removes trash files, i.e., the text aux files and the video and ******/
        /****** audio files before concatenation ******/
        for (uint8_t count = 1; count <= number_of_recordings; count++) {
            QFile::remove(rec_dir + filename + QString::number(count) + video_file_extension) ;
            QFile::remove(rec_dir + filename + QString::number(count) + ".mp3") ;
        }
        QFile::remove(rec_dir + "audio_list.txt") ;
        QFile::remove(rec_dir + "video_list.txt") ;

        /**************** Upload Routine Here ****************/
        /**************** Upload Routine Here ****************/
        /**************** Upload Routine Here ****************/
        /**************** Upload Routine Here ****************/
        /**************** Upload Routine Here ****************/
    }
    else if(ret == QMessageBox::No) {
        /****** Removes trash files, i.e., the text aux files and the video and ******/
        /****** audio files before concatenation ******/
        for (uint8_t count = 1; count <= number_of_recordings; count++) {
            QFile::remove(rec_dir + filename + QString::number(count) + video_file_extension) ;
            QFile::remove(rec_dir + filename + QString::number(count) + ".mp3") ;
        }
        QFile::remove(rec_dir + "audio_list.txt") ;
        QFile::remove(rec_dir + "video_list.txt") ;
    }
}


/************* PUSH_BUTTON *************/
// Defines behaviour of the "RECORD" push button
void MainDialog::on_pushButton_2_clicked()
{
    number_of_recordings++ ;

    /******* If checkbox is checked, change to media mode, i.e., fps=30 *******/
    /******* Else fps=1 for keeping the video output small *******/
    if(ui->checkBox->isChecked()) {
        fps = "30" ;
    }
    else{
        fps = "1" ;
    }

    ui->label->setStyleSheet("color: green");
    ui->label->setText("Status: RECORDING");
    ui->pushButton_4->setEnabled(true);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_5->setEnabled(true);
    ui->checkBox->setEnabled(false);

    /************** Process used to call ffmpeg from terminal **************/
    ffmpeg_process = new QProcess(this) ;
    QStringList ffmpeg_args ;

    full_path = rec_dir + filename + QString::number(number_of_recordings) ;

                /************ Video ************/
    ffmpeg_args << "-y"
                << "-rtbufsize"
                << "1500M"
                << "-framerate"
                << fps
                << "-f"
                << "dshow"
                << "-i"
                << "video=screen-capture-recorder"
                << "-vcodec"
                << "libx264"
                << "-an"                            // no audio option
                << full_path + video_file_extension
                //<< "-s"
                //<< "2880x1800"

                /************ Audio ************/
                << "-f"
                << "dshow"
                << "-i"
                << "audio=Internal Microphone (Cirrus Logic CS4208 (AB 94))"
                //<< "-ac"
                //<< "2"
                << "-acodec"
                << "libmp3lame"
                << "-qscale:a"
                << "9"
                << full_path + audio_file_extension ;


    //qDebug() << ffmpeg_args ;

    /* Useful for recording on Linux
     * ffmpeg -framerate 25 -video_size 1024x768 -f x11grab -i :0.0+100,200 -f
     * alsa -ac 2 -i pulse -vcodec libx264 -crf 0 -preset ultrafast -acodec pcm_s16le output.mkv
     *
     * ffmpeg -rtbufsize 1500M -framerate 30 -f dshow -i video=screen-capture-recorder -f dshow -i audio="Microphone (Parallels Audio Controller)" -ac 2 -vcodec libx264 -crf 0 -preset ultrafast -acodec pcm_s16le output.wav
     */

    connect(ffmpeg_process, SIGNAL(finished(int,QProcess::ExitStatus)), ffmpeg_process, SLOT(deleteLater())) ;
    ffmpeg_process->start("ffmpeg", ffmpeg_args) ;
}


/************* PUSH_BUTTON *************/
// Defines behaviour of the "STOP RECORDING" push button
void MainDialog::on_pushButton_4_clicked() {

    ui->label->setStyleSheet("color: red");
    ui->label->setText("Status: NOT RECORDING");

    ui->pushButton_4->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->checkBox->setEnabled(true);


    ffmpeg_process->setProcessChannelMode(QProcess::ForwardedChannels);
    ffmpeg_process->write("q");
    ffmpeg_process->closeWriteChannel();

}


/************* PUSH_BUTTON *************/
// Defines behaviour of the "DOCUMENT CAMERA" push button
void MainDialog::on_pushButton_5_clicked()
{
    ui->label->setStyleSheet("color: orange") ;
    ui->label->setText("Status: CAMERA DOCUMENT ON");

    netcat_mplayer_client = new QProcess(this) ;
    netcat_mplayer_client->startDetached("C:/Users/luigu/Desktop/tcc_gui/tcc_gui/netcat_mplayer_client.bat") ;

    rpi_cam = new QProcess(this) ;
    rpi_cam->start("C:/Users/luigu/Desktop/tcc_gui/tcc_gui/rpi_cam.bat") ;

}


void MainDialog::on_pushButton_6_clicked() {
    upload = new QProcess(this) ;
    upload->start("C:/Users/luigu/Desktop/tcc_gui/tcc_gui/transfer_test.bat") ;
    upload->waitForFinished(-1) ;

}
