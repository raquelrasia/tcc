#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mydialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->menuBar->setNativeMenuBar(false) ;

    this->setWindowTitle("Lecture Recorder");

    /* SECAO ESQUERDA */
    connect(ui->actionNova_Gravacao, SIGNAL(triggered(bool)), ui->pushButton, SLOT(click())) ;
    connect(ui->actionEncerrar_Gravacao, SIGNAL(triggered(bool)), ui->pushButton_3, SLOT(click())) ;
    ui->actionEncerrar_Gravacao->setEnabled(false);
    ui->pushButton_3->setEnabled(false);

    /* SECAO DIREITA */
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_4->setEnabled(false);

    /* STATUS */
    ui->label->setStyleSheet("color: red") ;
    ui->label->setText("Status: NOT RECORDING") ;

    font = ui->label->font() ;
    font.setPointSize(14);
    ui->label->setFont(font);

    dialog_ended = false ;
    number_of_recordings = 0 ;
    fps = "1" ;
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* NOVA GRAVACAO */
void MainWindow::on_pushButton_clicked()
{
    if(number_of_recordings != 0) {
        for (uint8_t count = 1; count <= number_of_recordings; count++) {
            QFile::remove(path + filename + QString::number(count) + video_file_type) ;
        }
        QFile::remove(path + "mylist.txt") ;
        number_of_recordings = 0 ;
    }

    MyDialog mDialog ;
    mDialog.setModal(true) ;
    mDialog.exec() ;

    if(mDialog.get_dialog_ended()) {
        dialog_ended = mDialog.get_dialog_ended() ;
        ui->pushButton->setEnabled(false);
        ui->actionNova_Gravacao->setEnabled(false);
        ui->pushButton_3->setEnabled(true);
        ui->actionEncerrar_Gravacao->setEnabled(true);
        ui->pushButton_2->setEnabled(true);

        path = mDialog.get_path() ;
        filename = mDialog.get_filename() ;
        video_file_type = mDialog.get_file_type() ;
    }
}

/* Encerrar Gravacao */
void MainWindow::on_pushButton_3_clicked()
{

    if(QFileInfo::exists(path + filename + QString::number(1) + video_file_type) ) {

        /*********** Cria Arquivo de texto com videos a serem concatenados ***********/
        QString mylistname = path + QString("mylist.txt") ;
        QFile file(mylistname);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                return;

        QTextStream out(&file);
        for (uint8_t count = 1; count <= number_of_recordings; count++) {
            out << "file '" << filename + QString::number(count) + video_file_type << "'\n";
        }
        /****************************************************************************/

        /*********************** Concatena arquivos de video ************************/
        ffmpeg_cat = new QProcess(this) ;
        QStringList ffmpeg_cat_args ;
        ffmpeg_cat_args << "-f"
                        << "concat"
                        << "-i"
                        << path + QString("mylist.txt")
                        << "-c"
                        << "copy"
                        << path + filename + video_file_type ;

        //connect(ffmpeg_cat, SIGNAL(finished(int,QProcess::ExitStatus)), ffmpeg_cat, SLOT(deleteLater())) ;
        ffmpeg_cat->startDetached("ffmpeg", ffmpeg_cat_args) ;
        ffmpeg_cat->waitForFinished() ;
        ffmpeg_cat->close();
        qDebug() << ffmpeg_cat->state() ;
        qDebug() << ffmpeg_process->state() ;
        /**************************************************************************/

    }

    ui->pushButton->setEnabled(true) ;
    ui->actionNova_Gravacao->setEnabled(true);
    ui->pushButton_2->setEnabled(false) ;
    ui->pushButton_3->setEnabled(false) ; // encerrar
    ui->actionEncerrar_Gravacao->setEnabled(false);
    ui->pushButton_4->setEnabled(false) ;
    ui->label->setStyleSheet("color: orange");
    ui->label->setText("Status: Lecture Finished");
}


/* Gravar */
void MainWindow::on_pushButton_2_clicked()
{
    number_of_recordings++ ;

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

    ffmpeg_process = new QProcess(this) ;
    QStringList ffmpeg_args ;

    full_path = path + filename + QString::number(number_of_recordings) ;

    ffmpeg_args //<< "-rtbufsize"
                //<< "1500M"
                /*<< "-framerate"
                << fps
                << "-f"
                << "dshow"
                << "-i"
                << "video=screen-capture-recorder"
                << "-vcodec"
                << "libx264"
                << full_path + video_file_type*/

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
                //<< "-s"
                //<< "2880x1800"

    qDebug() << ffmpeg_args ;

    /*
     * ffmpeg -framerate 25 -video_size 1024x768 -f x11grab -i :0.0+100,200 -f
     * alsa -ac 2 -i pulse -vcodec libx264 -crf 0 -preset ultrafast -acodec pcm_s16le output.mkv
     *
     * ffmpeg -rtbufsize 1500M -framerate 30 -f dshow -i video=screen-capture-recorder -f dshow -i audio="Microphone (Parallels Audio Controller)" -ac 2 -vcodec libx264 -crf 0 -preset ultrafast -acodec pcm_s16le output.wav
     */

    //qDebug() << ffmpeg_args ;
    connect(ffmpeg_process, SIGNAL(finished(int,QProcess::ExitStatus)), ffmpeg_process, SLOT(deleteLater())) ;
    //ffmpeg_process->startDetached("ffmpeg", ffmpeg_args) ;
    ffmpeg_process->start("ffmpeg", ffmpeg_args) ;
}


/* Parar */
void MainWindow::on_pushButton_4_clicked() {

    ui->label->setStyleSheet("color: red");
    ui->label->setText("Status: NOT RECORDING");

    ui->pushButton_4->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);

    ffmpeg_process->setProcessChannelMode(QProcess::ForwardedChannels);
    ffmpeg_process->write("q");
    ffmpeg_process->closeWriteChannel();

}


/* Sair */
void MainWindow::on_actionSair_triggered()
{
    qApp->quit();
}

/* Delete arquivos temporarios quando sair */
void MainWindow::closeEvent(QCloseEvent *event) {
    for (uint8_t count = 1; count <= number_of_recordings; count++) {
        QFile::remove(path + filename + QString::number(count) + video_file_type) ;
    }
    QFile::remove(path + "mylist.txt") ;

    event->accept();
}

/* Verifica se caminho existe */
bool check_path(QString path) {
    QFileInfo check_file(path);

    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}
