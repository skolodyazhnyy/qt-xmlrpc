#ifndef XMLRPCABSTRACT_H
#define XMLRPCABSTRACT_H

#include <QDomElement>
#include <QVariant>
#include <QVariantList>
#include <QString>

class XmlRpcAbstract {
protected:
    QVariantList parseXmlRpcParams(const QDomElement &element);
    QVariant parseXmlRpcValue(const QDomElement &element);
    QVariant parseXmlRpcStruct(const QDomElement &element);
    QVariant parseXmlRpcArray(const QDomElement &element);
    QString toXmlRpcFault(const QVariant &child);
    QString toXmlRpcParams(const QVariantList &child);
    QString toXmlRpcValue(const QVariant &child);
    QString toXmlRpcArray(const QVariantList &child);
    QString toXmlRpcStruct(const QVariantMap &child);
};

#endif // XMLRPCABSTRACT_H
