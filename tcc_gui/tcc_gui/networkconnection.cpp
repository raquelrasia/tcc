#include "networkconnection.h"

NetworkConnection::NetworkConnection()
{
    mgr = new QNetworkAccessManager();
}

int NetworkConnection::loginRequest(QString username, QString password){
    QString str;
    str.append(BASE_URL);
    str.append(LOGIN_URL);
    // create custom temporary event loop on stack
    QEventLoop eventLoop;
    // "quit()" the event-loop, when the network request "finished()"
    QObject::connect(mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    QNetworkRequest req( QUrl( QString(LOGIN_TOTAL_URL)));
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    str.clear();
    str.append("username=");
    str.append(username);
    str.append("&");
    QByteArray postData;
    postData.append(str);
    str.clear();
    str.append("password=");
    str.append(password);
    postData.append(str);

    QNetworkReply * reply = mgr->post(req, postData);
    eventLoop.exec(); // blocks stack until "finished()" has been called
    int result = reply->readAll().toInt();

    qDebug() << result;
    if (reply->error() == QNetworkReply::NoError)
    {
        //success
        if (result == RESPONSE_ERROR)
        {
            errorCause = "Your Login or Password is Incorrect";
        }
        delete reply;
    }
    else
    {
        //failure
        errorCause = reply->errorString();
        result = RESPONSE_ERROR;
        delete reply;
    }

    return result;
}


QString NetworkConnection::downloadXmlTeacherFile(QString username, QString xml_path)
{
    QString str;
    str.append(BASE_URL);
    str.append(LOGIN_URL);
    str.clear();
    // create custom temporary event loop on stack
    QEventLoop eventLoop;
    // "quit()" the event-loop, when the network request "finished()"
    QObject::connect(mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QUrl url = QUrl( QString(TEACHER_FILE_TOTAL_URL));

    QUrlQuery query;
    query.addQueryItem("teacher_username", username);
    url.setQuery(query.query());

    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QNetworkReply * reply = mgr->get(req);

    eventLoop.exec(); // blocks stack until "finished()" has been called

    QFile file(xml_path);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << reply->readAll() << endl;
    }

    return xml_path;
}
