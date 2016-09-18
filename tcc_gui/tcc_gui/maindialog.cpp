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
        MyDialog mDialog ;
        mDialog.setModal(true) ;
        mDialog.exec() ;

        if(mDialog.get_dialog_ended()) {
            dialog_ended = mDialog.get_dialog_ended() ;
            ui->pushButton->setEnabled(false);
            ui->pushButton_3->setEnabled(true);
            ui->pushButton_2->setEnabled(true);

            path = mDialog.get_path() ;
            filename = mDialog.get_filename() ;
            video_file_extension = mDialog.get_file_extension() ;
        }
    }

    // If automatic path is enabled, the user
    // chooses the course and the class and the
    // program automatically creates the recording
    // directory
    else {
        MyCourseDialog mcDialog ;
        MyCourseDialog mcDialog(0, xml_path) ;
        mcDialog.setModal(true) ;
        mcDialog.exec() ;

        if(mcDialog.get_dialog_ended()) {
            course_code = mcDialog.get_current_course() ;
            class_code = mcDialog.get_current_class() ;

            autopath_dialog_ended = mcDialog.get_dialog_ended() ;
            ui->pushButton->setEnabled(false);
            ui->pushButton_3->setEnabled(true);
            ui->pushButton_2->setEnabled(true);

            path = mcDialog.get_path() ;
            filename = mcDialog.get_filename() ;
            video_file_extension = mcDialog.get_file_extension() ;

            qDebug() << path << filename << video_file_extension ;
        }


    }
}


/************* PUSH_BUTTON *************/
// Defines behaviour of the "END LECTURE" push button
void MainDialog::on_pushButton_3_clicked()
{

    if(QFileInfo::exists(path + filename + QString::number(1) + video_file_extension) ) {

        /*********** Creates a text file with the video files to be concatenated ***********/
        QString video_list = path + QString("video_list.txt") ;
        QFile filevideo(video_list);
            if (!filevideo.open(QIODevice::WriteOnly | QIODevice::Text))
                return;

        QTextStream outvideo(&filevideo);
        for (uint8_t count = 1; count <= number_of_recordings; count++) {
            outvideo << "file '" << filename + QString::number(count) + video_file_extension << "'\n";
        }
        /****************************************************************************/

        /*********** Creates a text file with the audio files' to be concatenated ***********/
        QString audio_list = path + QString("audio_list.txt") ;
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
                        << path + QString("video_list.txt")
                        << "-c"
                        << "copy"
                        << "-an"
                        << path + filename + video_file_extension

                        /******************** Audio ********************/
                        << "-f"
                        << "concat"
                        << "-i"
                        << path + QString("audio_list.txt")
                        << "-c"
                        << "copy"
                        << path + filename + ".mp3" ;

        //connect(ffmpeg_cat, SIGNAL(finished(int,QProcess::ExitStatus)), ffmpeg_cat, SLOT(deleteLater())) ;
        ffmpeg_cat->startDetached("ffmpeg", ffmpeg_cat_args) ;
        ffmpeg_cat->waitForFinished() ;
        ffmpeg_cat->close();
        qDebug() << ffmpeg_cat->state() ;
        qDebug() << ffmpeg_process->state() ;
        /**************************************************************************/

    }

    ui->pushButton->setEnabled(true) ;
    ui->pushButton_2->setEnabled(false) ;
    ui->pushButton_3->setEnabled(false) ;
    ui->pushButton_4->setEnabled(false) ;
    ui->label->setStyleSheet("color: orange");
    ui->label->setText("Status: Lecture Finished");

    /*********** Displays message asking if user wants to upload video now ***********/
    QMessageBox msgBox ;
    msgBox.setInformativeText("Do you wish to upload the lecture now?") ;
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();

    if(ret == QMessageBox::Yes){
        /****** Removes trash files, i.e., the text aux files and the video and ******/
        /****** audio files before concatenation ******/
        for (uint8_t count = 1; count <= number_of_recordings; count++) {
            QFile::remove(path + filename + QString::number(count) + video_file_extension) ;
            QFile::remove(path + filename + QString::number(count) + ".mp3") ;
        }
        QFile::remove(path + "audio_list.txt") ;
        QFile::remove(path + "video_list.txt") ;

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
            QFile::remove(path + filename + QString::number(count) + video_file_extension) ;
            QFile::remove(path + filename + QString::number(count) + ".mp3") ;
        }
        QFile::remove(path + "audio_list.txt") ;
        QFile::remove(path + "video_list.txt") ;
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
    ui->checkBox->setEnabled(false);

    /************** Process used to call ffmpeg from terminal **************/
    ffmpeg_process = new QProcess(this) ;
    QStringList ffmpeg_args ;

    full_path = path + filename + QString::number(number_of_recordings) ;

    qDebug() << full_path ;

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
                << "audio=Microphone (Parallels Audio Controller)"
                //<< "-ac"
                //<< "2"
                << "-acodec"
                << "libmp3lame"
                << "-qscale:a"
                << "9"
                << full_path + ".mp3" ;


    qDebug() << ffmpeg_args ;

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

 void MainDialog::set_xml_path(QString path)
 {
    xml_path = path;
    qDebug()<< xml_path;
 }
