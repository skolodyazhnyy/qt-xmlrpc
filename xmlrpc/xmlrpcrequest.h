#ifndef XMLRPCREQUEST_H
#define XMLRPCREQUEST_H

#include <QString>
#include <QVariant>
#include "xmlrpcabstract.h"

class XmlRpcRequest : public XmlRpcAbstract {
    QString _methodname;
    QVariantList _params;
public:
    XmlRpcRequest();
    XmlRpcRequest(QString name, QVariantList params);
    static XmlRpcRequest* fromString(QByteArray data);
    QString toString();

    QString methodName();
    XmlRpcRequest* setMethodName(QString method_name);

    QVariantList params();
    XmlRpcRequest *setParams(QVariantList params);
    XmlRpcRequest *setParams(const QDomElement &element);
};

#endif // XMLRPCREQUEST_H
