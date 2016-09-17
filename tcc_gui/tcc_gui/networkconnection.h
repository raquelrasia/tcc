
#ifndef NETWORKCONNECTION_H
#define NETWORKCONNECTION_H


#define BASE_URL                    "http://192.168.0.114:8000/"
#define LOGIN_URL                   "login/"
#define LOGIN_TOTAL_URL             "http://192.168.0.114:8000/api/login/"
#define TEACHER_FILE_TOTAL_URL      "http://192.168.0.114:8000/api/teacher_xml_download/"

#define NO_ERROR_CODE        1
#define RESPONSE_ERROR      -1

#include <QCoreApplication>
#include <QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QFile>
#include <QStandardPaths>
#include <QDir>

class NetworkConnection
{
    public:
        NetworkConnection();
        int loginRequest(QString username, QString password);
        QString downloadXmlTeacherFile(QString username, QString xml_path);
        QString errorCause;
    private:
        QNetworkAccessManager * mgr;
};

#endif // NETWORKCONNECTION_H
