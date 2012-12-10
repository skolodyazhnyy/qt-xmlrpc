#include "xmlrpcresponse.h"
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QVariantList>

XmlRpcResponse::XmlRpcResponse() {
    _result.clear();
}

XmlRpcResponse *XmlRpcResponse::fromString(QByteArray data) {
    XmlRpcResponse *xmlrpc_response = new XmlRpcResponse();
    QDomDocument doc;
    if(doc.setContent(data)) {
        QDomElement methodResponse = doc.documentElement();
        if(methodResponse.isNull() || methodResponse.tagName() != "methodResponse") {
            return NULL;
        } else {
            for(QDomNode node = methodResponse.firstChild(); !node.isNull(); node = node.nextSibling()) {
                if(!node.isElement()) continue;
                QDomElement element = node.toElement();
                if(element.tagName() == "params") {
                    xmlrpc_response->setResult(element);
                } else if(element.tagName() == "fault") {
                    if(element.hasChildNodes()) {
                        QDomElement fault_value = element.firstChild().toElement();
                        if(fault_value.tagName() == "value")
                            xmlrpc_response->setFault(fault_value);
                        else
                            xmlrpc_response->setFault("Unknown error", 0);
                    } else
                        xmlrpc_response->setFault("Unknown error", 0);
                }
            }
        }
    } else {
        return NULL;
    }
    return xmlrpc_response;
}

QString XmlRpcResponse::toString() {
    QString response_data;

    if(isFault()) {
        response_data = toXmlRpcFault(fault());
    } else {
        QVariantList params;
        if(result().isValid())
            params.append(result());
        response_data = toXmlRpcParams(params);
    }

    return "<methodResponse>" + response_data + "</methodResponse>";
}

QVariant XmlRpcResponse::result() {
    if(!isSuccess())
        return QVariant::Invalid;
    return _result;
}

XmlRpcResponse *XmlRpcResponse::setResult(const QDomElement &element) {
    return setResult(parseXmlRpcParams(element).first());
}

XmlRpcResponse *XmlRpcResponse::setResult(QVariant result) {
    _isFault = false;
    _result = result;
    return this;
}

bool XmlRpcResponse::isSuccess() {
    return !_isFault;
}

XmlRpcResponse *XmlRpcResponse::setFault(const QDomElement &element) {
    return setFault(parseXmlRpcValue(element));
}

XmlRpcResponse *XmlRpcResponse::setFault(QString error, qint32 errno) {
    QVariantMap fault;
    fault["faultCode"] = errno;
    fault["faultString"] = error;
    return setFault(fault);
}

XmlRpcResponse *XmlRpcResponse::setFault(QVariant fault) {
    _isFault = true;
    _result = fault;
    return this;
}

QString XmlRpcResponse::faultString() {
    if(!isFault())
        return 0;
    QVariantMap f = fault().toMap();
    if(!f.contains("faultString"))
        return 0;
    return f["faultString"].toString();
}

qint32 XmlRpcResponse::faultCode() {
    if(!isFault())
        return 0;
    QVariantMap f = fault().toMap();
    if(!f.contains("faultCode"))
        return 0;
    return f["faultCode"].toInt();
}

QVariant XmlRpcResponse::fault() {
    if(!isFault())
        return QVariant::Invalid;
    return _result;
}

bool XmlRpcResponse::isFault() {
    return _isFault;
}

