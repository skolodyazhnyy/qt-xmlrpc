#ifndef XMLRPCCLIENT_H
#define XMLRPCCLIENT_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "xmlrpcresponse.h"
#include "xmlrpcrequest.h"

class XmlRpcClient : public QObject
{
    Q_OBJECT
    QString address;
    qint32 port;
    QNetworkRequest *network_request;
public:
    XmlRpcClient(QString address);
    XmlRpcResponse* execute(XmlRpcRequest *network_request, bool async = true);
    XmlRpcResponse* execute(QString methodname, QVariantList params, bool async = true);
    XmlRpcClient(QNetworkRequest *network_request);
    XmlRpcClient *setNetworkRequest(QNetworkRequest *network_request);
protected slots:
    void slotReplyFinished(QNetworkReply *reply);
    void slotError(QNetworkReply::NetworkError err);
   // void slotSslErrors(QList<QSslError> errors);

signals:
    void complete(XmlRpcResponse *);
    void error(QNetworkReply::NetworkError error);
   // void sslError(QList<QSslError> errors);

};

#endif // XMLRPCCLIENT_H
