#ifndef XMLRPCPACKAGE_H
#define XMLRPCPACKAGE_H

#include <QString>
#include <QVariantList>
#include <QDomElement>
#include "xmlrpcabstract.h"

class XmlRpcPackage : public XmlRpcAbstract
{
protected:
    QString _name;
    QVariantList _params;
    XmlRpcPackage *setParams(const QDomElement &element);
public:
    XmlRpcPackage();
    XmlRpcPackage(QString name);
    XmlRpcPackage(QString name, QVariantList params);
    static XmlRpcPackage *fromString(QByteArray data);
    QString toString();
    QString name();
    XmlRpcPackage *setName(QString name);
    QVariantList params();
    XmlRpcPackage *setParams(QVariantList params);
    XmlRpcPackage *addParam(QVariant param);
    QByteArray toByteArray();
};

#endif // XMLRPCPACKAGE_H
