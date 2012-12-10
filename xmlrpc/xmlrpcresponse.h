#ifndef XMLRPCRESPONSE_H
#define XMLRPCRESPONSE_H

#include "xmlrpcabstract.h"
#include <QString>
#include <QVariant>

class XmlRpcResponse : public XmlRpcAbstract
{
    QVariant _result;
    bool _isFault;
public:
    XmlRpcResponse();
    QString toString();

    static XmlRpcResponse *fromString(QByteArray data);

    XmlRpcResponse *setFault(QString error, qint32 errno);
    XmlRpcResponse *setFault(const QDomElement &element);
    XmlRpcResponse *setFault(QVariant fault);
    QString faultString();
    qint32 faultCode();
    QVariant fault();
    bool isFault();


    XmlRpcResponse *setResult(const QDomElement &element);
    XmlRpcResponse *setResult(QVariant result);
    QVariant result();
    bool isSuccess();
};

#endif // XMLRPCRESPONSE_H
