#include "xmlrpcclient.h"
#include <QDebug>
#include <QEventLoop>

XmlRpcClient::XmlRpcClient(QNetworkRequest *request) {
    setNetworkRequest(request);
}

XmlRpcClient::XmlRpcClient(QString address) {
    this->address = address;
}

XmlRpcClient* XmlRpcClient::setNetworkRequest(QNetworkRequest *request) {
    network_request = request;
    return this;
}

XmlRpcResponse *XmlRpcClient::execute(XmlRpcRequest *request, bool async) {
    QNetworkAccessManager *network_manager = new QNetworkAccessManager();
    QNetworkRequest network_request;
    network_request.setUrl(QUrl(this->address));
    network_request.setRawHeader("User-Agent", "XMLRPC browser");
    network_request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");

    //connect(network_manager, SIGNAL(finished(QNetworkReply*)),
      //      this, SLOT(slotReplyFinished(QNetworkReply*)));

    QNetworkReply *reply = network_manager->post(network_request, request->toString().toLatin1());

    //connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            //this, SLOT(slotError(QNetworkReply::NetworkError)));

    //connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
    //        this, SLOT(slotSslErrors(QList<QSslError>)));

    if(async)
        return NULL; // wait for complete signal

    QEventLoop loop;
    connect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));
    loop.exec();

    return XmlRpcResponse::fromString(reply->readAll());
}

XmlRpcResponse *XmlRpcClient::execute(QString methodname, QVariantList params, bool async) {
    return execute(new XmlRpcRequest(methodname, params), async);
}

void XmlRpcClient::slotReplyFinished(QNetworkReply *reply) {
    emit complete(XmlRpcResponse::fromString(reply->readAll()));
}

void XmlRpcClient::slotError(QNetworkReply::NetworkError err) {
    //emit error(err);
}

/*void XmlRpcClient::slotSslErrors(QList<QSslError> errors) {
    //emit sslError(errors);
}
*/
