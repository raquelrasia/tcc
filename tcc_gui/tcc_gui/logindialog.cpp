#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Login");
    // ---- change to config path ----//
    QString path = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation) + QString("/") ;
    QDir dir(path) ;
    dir.mkpath(path + "teacher_info") ;
    path = path + "teacher_info/" ;
    QString filename = "auth_info.xml";
    auth_file_path = path + filename;
    int days_expire = 50;
    bool ask_login = read_auth_xml(&username, &days_expire);
    if (!ask_login)
    {
        close() ;
        QString xml_path = make_xml_path();
        MainDialog mdialog ;
        mdialog.set_xml_path(xml_path);
        mdialog.setModal(true);
        mdialog.exec() ;
    }
    else
    {
        ui->expire_label->setVisible(false);
        if(days_expire >45)
        {
            ui->ContinueSession->setEnabled(false);
        }
        else
        {
            ui->expire_label->setVisible(true);
            ui->ContinueSession->setEnabled(true);
        }
        show();
    }
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_pushButton_clicked()
{
    QString username = ui->lineEdit->text() ;
    QString password = ui->lineEdit_2->text() ;

    NetworkConnection connection;
    //int errorCode = connection.loginRequest(username, password);
    int errorCode = -1;
    QString xml_path = make_xml_path();
    errorCode = connection.loginRequest(username, password);
    if(errorCode == NO_ERROR_CODE) {
        close() ;
        connection.downloadXmlTeacherFile(username, xml_path);
        //--- save file with authenticated status ---//
        write_auth_xml();

        MainDialog mdialog ;
        mdialog.set_xml_path(xml_path);
        mdialog.setModal(true);
        mdialog.exec() ;

    }
    else {
        QMessageBox::warning(this, "Login", connection.errorCause) ;
    }
}


void LoginDialog::closeEvent(QCloseEvent *event) {
    event->accept();
}

QString LoginDialog::make_xml_path()
{
    QString filename = username + ".xml";
    QString path = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation) + QString("/") ;
    QDir dir(path) ;
    dir.mkpath(path + "teacher_info") ;
    path = path + "teacher_info/" ;
    QString xml_path = path+filename;
    return xml_path;
}

void LoginDialog::write_auth_xml()
{

    QDate date = QDate::currentDate();
    QString date_string = date.toString("dd_MM_yyyy") ;

    QFile file(auth_file_path);
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("teacher");
    xmlWriter.writeAttribute("username", username);
    xmlWriter.writeTextElement("auth_state", "true");
    xmlWriter.writeTextElement("date", date_string);
    xmlWriter.writeEndElement();
    file.close();

}

bool LoginDialog::read_auth_xml(QString * username, int * days_expire)
{
    bool ask_login = true;
    bool delete_file = false;
    bool has_teacher = false;
    bool has_auth = false;
    bool has_date = false;
    QDate current_date = QDate::currentDate();

    QFile file(auth_file_path);
        if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Couldn't read file";
        return ask_login;
    }

    QXmlStreamReader xml(&file);
    QXmlStreamAttributes attributes = xml.attributes();
    xml.readNext();
    
    while (!xml.atEnd()) {
        if (xml.readNextStartElement())
        {
            if(xml.name() == "teacher")
            {
                attributes = xml.attributes();
                if (attributes.hasAttribute("username"))
                {
                    foreach(const QXmlStreamAttribute &attr, attributes) {
                        if (attr.name().toString() == "username") {
                            *username = attr.value().toString();
                            has_teacher = true;
                        }
                    }
                }
                else
                {
                    has_teacher = false;
                }
            }
            else if (xml.name() == "auth_state")
            {
                if(xml.readElementText() == "true")
                {
                    has_auth = true;
                }
            }
            else if (xml.name() == "date")
            {
                QDate last_date = QDate::fromString(xml.readElementText(), "dd_MM_yyyy") ;
                int days_count = current_date.daysTo(last_date);
                *days_expire = days_count;
                qDebug()<< *days_expire;
                if (days_count > 30)
                {
                    has_date = false;
                    if(days_count > 45)
                    {
                        delete_file = true;
                    }
                }
                else
                {
                    has_date = true;
                }
            }
         }
    }

    file.close();
    if (delete_file)
    {
        
    }
    if (has_teacher && has_auth && has_date)
    {
        ask_login = false;
    }

    return ask_login;
}

void LoginDialog::on_ContinueSession_clicked()
{
        close() ;
        QString xml_path = make_xml_path();
        MainDialog mdialog ;
        mdialog.set_xml_path(xml_path);
        mdialog.setModal(true);
        mdialog.exec() ;
}

