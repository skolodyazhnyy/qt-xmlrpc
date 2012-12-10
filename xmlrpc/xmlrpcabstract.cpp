#include "xmlrpcabstract.h"
#include <QDomElement>
#include <QVariant>
#include <QDateTime>
#include <QDebug>

QVariant XmlRpcAbstract::parseXmlRpcValue(const QDomElement &element) {
    if(element.tagName() != "value") return QVariant::Invalid;
    QDomElement type = element.firstChild().toElement();

    if(type.tagName() == "int" || type.tagName() == "i4")
        return type.firstChild().toText().data().toInt();
    if(type.tagName() == "boolean")
        return type.firstChild().toText().data() == "1" ? true : false;
    if(type.tagName() == "string")
        return type.firstChild().toText().data();
    if(type.tagName() == "double")
        return type.firstChild().toText().data().toDouble();
    if(type.tagName() == "dateTime.iso8601")
        return QDateTime::fromString(type.firstChild().toText().data(), "yyyyMMddTHH:mm:ss");
    if(type.tagName() == "base64")
        return QByteArray::fromBase64(type.firstChild().toText().data().toLatin1());
    if(type.tagName() == "array")
        return parseXmlRpcArray(type.firstChild().toElement());
    if(type.tagName() == "struct")
        return parseXmlRpcStruct(type);

    return QVariant::Invalid;
}

QVariant XmlRpcAbstract::parseXmlRpcStruct(const QDomElement &element) {
    QVariantMap map;
    for(QDomNode node = element.firstChild(); !node.isNull(); node = node.nextSibling()) {
        if(node.toElement().tagName() != "member") continue;
        QDomElement name = node.firstChild().toElement();
        if(name.tagName() != "name" ) continue;
        map[name.firstChild().toText().data()] = parseXmlRpcValue(name.nextSibling().toElement());
    }
    return map;
}

QVariant XmlRpcAbstract::parseXmlRpcArray(const QDomElement &element) {
    QVariantList list;
    if(element.tagName() == "data") {
        for(QDomNode node = element.firstChild(); !node.isNull(); node = node.nextSibling()) {
            list.append(parseXmlRpcValue(node.toElement()));
        }
    }
    return list;
}

QVariantList XmlRpcAbstract::parseXmlRpcParams(const QDomElement &element) {
    QVariantList list;
    if(element.tagName() == "params") {
        for(QDomNode node = element.firstChild(); !node.isNull(); node = node.nextSibling()) {
            if(!node.isElement() || node.toElement().tagName() != "param") continue;
            list.append(parseXmlRpcValue(node.firstChild().toElement()));
        }
    }
    return list;
}

QString XmlRpcAbstract::toXmlRpcParams(const QVariantList &child) {
    QString data;
    QListIterator< QVariant > i(child);
    while(i.hasNext())
        data.append("<param>" + toXmlRpcValue(i.next()) + "</param>");
    return "<params>" + data + "</params>";
}

QString XmlRpcAbstract::toXmlRpcFault(const QVariant &child) {
    return "<fault><value>" + toXmlRpcValue(child) + "</value></fault>";
}

QString XmlRpcAbstract::toXmlRpcValue(const QVariant &child) {
    switch(child.type()) {
        case QVariant::Int:
            return "<value><int>" + QString::number(child.toInt()) + "</int></value>";
        case QVariant::Bool:
            return "<value><boolean>" + QString(child.toBool() ? "1" : "0") + "</boolean></value>";
        case QVariant::String:
            return "<value><string>" + child.toString() + "</string></value>";
        case QVariant::Double:
            return "<value><double>" + QString::number(child.toDouble()) + "</double></value>";
        case QVariant::DateTime:
            return "<value><dateTime.iso8601>" + child.toDateTime().toString("yyyyMMddTHH:mm:ss") +
                      "</dateTime.iso8601></value>";
        case QVariant::ByteArray:
            return "<value><base64>" + child.toByteArray().toBase64() + "</base64></value>";
        case QVariant::List:
            return "<value><array><data>" + toXmlRpcArray(child.toList()) + "</data></array></value>";
        case QVariant::Map:
            return "<value><struct>" + toXmlRpcStruct(child.toMap()) + "</struct></value>";
        default:
            qCritical() << "toXmlRpcValue(): unknown return xmlrpc type"
                        << child.typeName() << endl << child;
    }
}

QString XmlRpcAbstract::toXmlRpcArray(const QVariantList &child) {
    QString data;
    QListIterator< QVariant > i(child);
    while(i.hasNext())
        data.append(toXmlRpcValue(i.next()));
    return data;
}

QString XmlRpcAbstract::toXmlRpcStruct(const QVariantMap &child) {
    QString data;
    QMapIterator<QString, QVariant> i(child);
    while(i.hasNext()) {
        i.next();
        data.append("<member><name>" + i.key() + "</name>" + toXmlRpcValue(i.value()) + "</member>");
    }
    return data;
}
