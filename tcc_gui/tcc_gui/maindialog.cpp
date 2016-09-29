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


/************* AUX METHOD *************/
// Overrides closeEvent method in order to
// make ethernet connection dynamic (default).
void MainDialog::closeEvent(QCloseEvent *event) {

    if(QFileInfo::exists(rec_dir + filename + QString::number(1) + video_file_extension) ) {

        if(!ui->checkBox_2->isChecked()) {
            QString base_dir = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation) + QString("/") ;
        }

        /*********** Creates a text file with the video files to be concatenated ***********/
        QString video_list = rec_dir + QString("video_list.txt") ;
        QFile filevideo(video_list);
            if (!filevideo.open(QIODevice::WriteOnly | QIODevice::Text))
                return;

        QTextStream outvideo(&filevideo);
        for (uint8_t count = 1; count <= number_of_recordings; count++) {
            outvideo << "file '" << filename + QString::number(count) + video_file_extension << "'\n";
        }
        filevideo.close();

        /****************************************************************************/

        /*********** Creates a text file with the audio files to be concatenated ***********/
        QString audio_list = rec_dir + QString("audio_list.txt") ;
        QFile fileaudio(audio_list);
            if (!fileaudio.open(QIODevice::WriteOnly | QIODevice::Text))
                return;

        QTextStream outaudio(&fileaudio);
        for (uint8_t count = 1; count <= number_of_recordings; count++) {
            outaudio << "file '" << filename + QString::number(count) + audio_file_extension << "'\n";
        }
        fileaudio.close();

        /****************************************************************************/


        /*********************** Concatenates video files ************************/
        QProcess * ffmpeg_cat_video = new QProcess(this) ;
        QStringList ffmpeg_video_cat_args ;

                        /******************** Video ********************/
        ffmpeg_video_cat_args << "-y"
                              << "-f"
                              << "concat"
                              << "-i"
                              << rec_dir + QString("video_list.txt")
                              << "-c"
                              << "copy"
                              << rec_dir + filename + video_file_extension ;

        //connect(ffmpeg_cat, SIGNAL(finished(int,QProcess::ExitStatus)), ffmpeg_cat, SLOT(deleteLater())) ;
        ffmpeg_cat_video->start("ffmpeg", ffmpeg_video_cat_args) ;
        ffmpeg_cat_video->waitForFinished() ;
        ffmpeg_cat_video->close();
        /**************************************************************************/

        /*********************** Concatenates Audio files ************************/
        QProcess * ffmpeg_cat_audio = new QProcess(this) ;
        QStringList ffmpeg_audio_cat_args ;

                        /******************** Audio ********************/
        ffmpeg_audio_cat_args << "-y"
                              << "-f"
                              << "concat"
                              << "-i"
                              << rec_dir + QString("audio_list.txt")
                              << "-c"
                              << "copy"
                              << rec_dir + filename + audio_file_extension ;

        //connect(ffmpeg_cat, SIGNAL(finished(int,QProcess::ExitStatus)), ffmpeg_cat, SLOT(deleteLater())) ;
        ffmpeg_cat_audio->start("ffmpeg", ffmpeg_audio_cat_args) ;
        ffmpeg_cat_audio->waitForFinished() ;
        ffmpeg_cat_audio->close();
        /**************************************************************************/
    }

    QProcess * network_setup_dynamic = new QProcess(this) ;

    qDebug() << program_dir ;
    network_setup_dynamic->start(program_dir + QString("config/") + QString("call_bat_admin.bat dynamic"));
    network_setup_dynamic->waitForFinished() ;
    network_setup_dynamic->close();

    for (uint8_t count = 1; count <= number_of_recordings; count++) {
        QFile::remove(rec_dir + filename + QString::number(count) + video_file_extension) ;
        QFile::remove(rec_dir + filename + QString::number(count) + audio_file_extension) ;
    }
    QFile::remove(rec_dir + "audio_list.txt") ;
    QFile::remove(rec_dir + "video_list.txt") ;

    event->accept();
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
    fps = "3" ;
    audio_file_extension = QString(".mp3") ;
    video_file_extension = QString(".webm") ;

    program_dir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + QString("/") + QString("LectureRecording/");

    QProcess * network_setup_static = new QProcess(this) ;

    network_setup_static->start(program_dir + QString("config/") + QString("call_bat_admin.bat static"));
    network_setup_static->waitForFinished() ;
    network_setup_static->close();
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
            year = mcDialog.get_current_year();
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
            ui->pushButton_5->setEnabled(true);
            ui->checkBox_2->setEnabled(false);

            rec_dir = mcDialog.get_rec_dir() ;
            dir_videos = mcDialog.get_dir_videos() ;
            semester = mcDialog.get_current_semester() ;
            filename = mcDialog.get_filename() ;
            year = mcDialog.get_current_year();
        }
    }
}



/************* PUSH_BUTTON *************/
// Defines behaviour of the "END LECTURE" push button
void MainDialog::on_pushButton_3_clicked()
{
    if(QFileInfo::exists(rec_dir + filename + QString::number(1) + video_file_extension) ) {

        if(ui->checkBox_2->isChecked()) {

            /*********** Creates a .bat file with the transfer routine if autopath is used ***********/
            QString transfer_cmds = program_dir + QString("config/") + QString("transfer.bat") ;
            qDebug() << transfer_cmds ;
            QFile bat_file(transfer_cmds);
                if (!bat_file.open(QIODevice::WriteOnly | QIODevice::Text))
                    return;

            QTextStream outcmds(&bat_file);
            outcmds << "plink -ssh pi@169.254.96.87 -pw raspberry -batch  \"mkdir -p /home/pi/videos/" << course_code +
                       QString("/") + class_code + QString("/") + year+QString("_") +semester + QString("/") + "\"\n" +
                       QString("pscp -pw raspberry ") + rec_dir + filename + video_file_extension + QString(" ") +
                       rec_dir + filename + audio_file_extension + QString(" ") + QString(" pi@169.254.96.87:videos/") +
                       course_code + QString("/") + class_code + QString("/") +  year+QString("_")+ semester + QString("/")  << "\n" ;
            bat_file.close();
            /**************************************************************************/
        }

        else {

            QString base_dir = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation) + QString("/") ;
            /*********************************************************************************************/
            /*********************************************************************************************/
            /*********************************************************************************************/
            /*********** Creates a .bat file with the transfer routine if autopath is NOT used ***********/
            QString transfer_cmds = program_dir + QString("config/") + QString("transfer.bat") ;
            QFile bat_file(transfer_cmds);
                if (!bat_file.open(QIODevice::WriteOnly | QIODevice::Text))
                    return;

            QTextStream outcmds(&bat_file);
            outcmds << "plink -ssh pi@169.254.96.87 -pw raspberry -batch \"mkdir -p /home/pi/videos/" << course_code + QString("/") +
                    class_code + QString("/") + year + QString("_") + semester + QString("/") + "\"\n" +
                    QString("pscp -pw raspberry ") + rec_dir + filename + video_file_extension + QString(" ") +
                    rec_dir + filename + audio_file_extension + QString(" ") +
                    QString(" pi@169.254.96.87:videos/") + course_code + QString("/") +
                    class_code + QString("/") +  year+QString("_") +semester + QString("/")  << "\n" ;
            bat_file.close();
            /**************************************************************************/
        }

        /*********** Creates a text file with the video files to be concatenated ***********/
        QString video_list = rec_dir + QString("video_list.txt") ;
        QFile filevideo(video_list);
            if (!filevideo.open(QIODevice::WriteOnly | QIODevice::Text))
                return;

        QTextStream outvideo(&filevideo);
        for (uint8_t count = 1; count <= number_of_recordings; count++) {
            outvideo << "file '" << filename + QString::number(count) + video_file_extension << "'\n";
        }
        filevideo.close();

        /****************************************************************************/

        /*********** Creates a text file with the audio files to be concatenated ***********/
        QString audio_list = rec_dir + QString("audio_list.txt") ;
        QFile fileaudio(audio_list);
            if (!fileaudio.open(QIODevice::WriteOnly | QIODevice::Text))
                return;

        QTextStream outaudio(&fileaudio);
        for (uint8_t count = 1; count <= number_of_recordings; count++) {
            outaudio << "file '" << filename + QString::number(count) + audio_file_extension << "'\n";
        }
        fileaudio.close();

        /****************************************************************************/


        /*********************** Concatenates video files ************************/
        QProcess * ffmpeg_cat_video = new QProcess(this) ;
        QStringList ffmpeg_video_cat_args ;

                        /******************** Video ********************/
        ffmpeg_video_cat_args << "-y"
                              << "-f"
                              << "concat"
                              << "-i"
                              << rec_dir + QString("video_list.txt")
                              << "-c"
                              << "copy"
                              << rec_dir + filename + video_file_extension ;

        //connect(ffmpeg_cat, SIGNAL(finished(int,QProcess::ExitStatus)), ffmpeg_cat, SLOT(deleteLater())) ;
        ffmpeg_cat_video->start("ffmpeg", ffmpeg_video_cat_args) ;
        ffmpeg_cat_video->waitForFinished() ;
        ffmpeg_cat_video->close();
        /**************************************************************************/

        /*********************** Concatenates Audio files ************************/
        QProcess * ffmpeg_cat_audio = new QProcess(this) ;
        QStringList ffmpeg_audio_cat_args ;

                        /******************** Audio ********************/
        ffmpeg_audio_cat_args << "-y"
                              << "-f"
                              << "concat"
                              << "-i"
                              << rec_dir + QString("audio_list.txt")
                              << "-c"
                              << "copy"
                              << rec_dir + filename + audio_file_extension ;

        //connect(ffmpeg_cat, SIGNAL(finished(int,QProcess::ExitStatus)), ffmpeg_cat, SLOT(deleteLater())) ;
        ffmpeg_cat_audio->start("ffmpeg", ffmpeg_audio_cat_args) ;
        ffmpeg_cat_audio->waitForFinished() ;
        ffmpeg_cat_audio->close();
        /**************************************************************************/
    }
    write_remote_xml_cmd_file(filename + video_file_extension, filename + audio_file_extension);
    ui->pushButton->setEnabled(true) ;
    ui->pushButton_2->setEnabled(false) ;
    ui->pushButton_3->setEnabled(false) ;
    ui->pushButton_4->setEnabled(false) ;
    ui->pushButton_5->setEnabled(false) ;
    ui->checkBox_2->setEnabled(true);
    ui->label->setStyleSheet("color: orange");
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
            QFile::remove(rec_dir + filename + QString::number(count) + audio_file_extension) ;
        }
        QFile::remove(rec_dir + "audio_list.txt") ;
        QFile::remove(rec_dir + "video_list.txt") ;

        ui->label->setStyleSheet("color: orange");
        ui->label->setText("Status: Please Wait, Transfering Files...");

        transfer_files = new QProcess(this) ;
        QObject::connect(transfer_files, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(on_finishedTransfer(int , QProcess::ExitStatus ))) ;
        transfer_files->start(program_dir + QString("config/") + QString("transfer.bat")) ;
        //transfer->waitForFinished();
        //transfer->close();
        ui->label->setStyleSheet("color: orange");
        ui->label->setText("Status: Lecture Finished");
    }
    else if(ret == QMessageBox::No) {
        /****** Removes trash files, i.e., the text aux files and the video and ******/
        /****** audio files before concatenation ******/
        for (uint8_t count = 1; count <= number_of_recordings; count++) {
            QFile::remove(rec_dir + filename + QString::number(count) + video_file_extension) ;
            QFile::remove(rec_dir + filename + QString::number(count) + audio_file_extension) ;
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
        fps = "3" ;
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

    // The commented section below refers to recording audio and video separately.
    // It's easier to record video with audio and then overwrite the audio channel
    // or simply make a copy of the audio channel to an audio container such as .mp3.
    // That way, one can have both "video with audio" and "audio" for the video and
    // podcast respectively.

                /************ Video ************/
  /*  ffmpeg_args << "-y"
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
                //<< "-an"                            // no audio option
                << full_path + video_file_extension
                //<< "-s"
                //<< "2880x1800"
*/
                /************ Audio ************/
  /*              << "-f"
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

*/
    // Records Video with Audio

                // Says "yes" to all questions
    ffmpeg_args << "-y"

                // rate buffer size. Important for real-time
                << "-rtbufsize"
                << "500M"

                << "-f"
                // Direct Show is Microsoft's streaming API
                << "dshow"

                << "-framerate"
                << fps

                // -i stands for input. So in this case, we have two inputs, a screen and a microphone
                << "-i"
                << "video=screen-capture-recorder:audio=Microphone (Realtek High Definition Audio)"
                // Video codec. It's the same as "-vcodec". CO-DEC stands for Compressor/Decompressor

                << "-c:v"
                // Since the video is been recorded for a web browser, we chose the video codec that is
                // compatible with all web browsers, i.e., the codec named VP8
                << "libvpx"

                // Same as "-framerate"
                << "-r"
                << fps

                // -deadline is a directive to select CPU use
                // realtime is the highest CPU use configuration for encoding
                << "-deadline"
                << "realtime"

                // Chooses goal video bitrate
                << "-b:v"
                << "98k"

                // Fixes bitrate to 98kbps
                << "-minrate"
                << "98k"
                << "-maxrate"
                << "98k"



                // Chooses color space to be YUV (YCbCr, which stands for Luminance, blue-difference and
                // red-difference chroma components)
                << "-pix_fmt"
                << "yuv420p"

                // Chooses the audio codec to be Vorbis, which is compatible with VB8 and the container WEBM
                << "-c:a"
                << "libvorbis"

                // Chooses goal audio bitrate. This is the lowest possible audio bitrate. Anything lower than
                // 32k will generate an error on FFMPEG
                << "-ab"
                << "32k"

                // Number of audio channels = 1 (mono). There's no apparent reason to use stereo recording
                << "-ac"
                << "1"

                // Chooses the audio sampling rate to be 22k
                << "-ar"
                << "22k"

                // output file. The container used is "webm"
                << full_path + video_file_extension


                // Records Audio

                << "-f"
                << "dshow"
                << "-i"
                << "audio=Microphone (Realtek High Definition Audio)"
                << "-c:a"
                << "libmp3lame"
                << "-ab"
                << "128k"
                << "-ac"
                << "1"
                << "-ar"

                // 44100Hz sampling rate
                << "44100"

                // .mp3 output file
                << full_path + audio_file_extension ;

                // ESTE ESTE ESTE ffmpeg -y -rtbufsize 500M -f dshow -framerate 5 -i video="screen-capture-recorder":audio="Internal Microphone (Cirrus Logic CS4208 (AB 94))" -c:v libvpx -preset ultrafast -r 5 -deadline realtime -b:v 98k -pix_fmt yuv420p -c:a libvorbis -ab 32k -ac 1 -ar 22k -vf "scale=640:360" "output.webm"
// ESTEAQUI ffmpeg -y -rtbufsize 500M -f dshow -framerate 3 -i video="screen-capture-recorder":audio="Internal Microphone (Cirrus Logic CS4208 (AB 94))" -c:v libvpx -preset ultrafast -r 3 -deadline realtime -b:v 98k -pix_fmt yuv420p -c:a libvorbis -ab 32k -ac 1 -ar 22k "output.webm"


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

    //ffmpeg_process->waitForFinished() ;
    //ffmpeg_process->close();
}


/************* PUSH_BUTTON *************/
// Defines behaviour of the "DOCUMENT CAMERA" push button
void MainDialog::on_pushButton_5_clicked()
{
    ui->label->setStyleSheet("color: orange") ;
    ui->label->setText("Status: CAMERA DOCUMENT ON");
    //ui->pushButton_5->setEnabled(false);

    QProcess * netcat_mplayer_client = new QProcess(this) ;
    netcat_mplayer_client->startDetached(program_dir + QString("config/") + QString("netcat_mplayer_client.bat")) ;

    QProcess * rpi_cam = new QProcess(this) ;
    rpi_cam->startDetached(program_dir + QString("config/") + QString("rpi_cam.bat")) ;
}


void MainDialog::on_pushButton_6_clicked() {
    QProcess * netcat_mplayer_client = new QProcess(this) ;
    netcat_mplayer_client->startDetached(program_dir + QString("config/") + QString("netcat_mplayer_client.bat")) ;

    QProcess * rpi_cam = new QProcess(this) ;
    rpi_cam->startDetached(program_dir + QString("config/") + QString("rpi_cam.bat")) ;
}

 void MainDialog::set_xml_path(QString path)
 {
    xml_path = path;
    qDebug()<< xml_path;
 }

 void MainDialog::set_username( QString usrname)
 {
    username = usrname;
 }

 bool MainDialog::write_remote_xml_cmd_file(QString video_name, QString audio_name)
 {
     QDate date = QDate::currentDate();
     QString lecture_date = date.toString("dd-MM-yyyy") ;
     QString transfer_cmds = program_dir + QString("config/") + QString("xml_sync.bat") ;
     QFile bat_file(transfer_cmds);
         if (!bat_file.open(QIODevice::WriteOnly | QIODevice::Text))
             return false;
     QTextStream outcmds(&bat_file);
     outcmds << "plink -ssh pi@169.254.96.87 -pw raspberry -batch \"python /home/pi/client/video_xml_manager.py "
             << course_code + QString(" ") + class_code + QString(" ")+ year + QString(" ") +
                semester +QString(" ") +lecture_date +QString(" ") + video_name + QString(" ") +
                audio_name + QString(" ") + username <<" \n pause \n" ;
     bat_file.close();
     return true;
 }

 void MainDialog::on_finishedTransfer(int exitCode, QProcess::ExitStatus exitStatus)
 {
    qDebug() << "Trasnfer Finished";
    QProcess * transfer = new QProcess(this) ;
    transfer->startDetached(program_dir + QString("config/") + QString("xml_sync.bat")) ;
    transfer_files->close();
 }

