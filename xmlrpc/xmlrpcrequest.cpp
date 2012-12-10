#include "xmlrpcrequest.h"
#include <QDomElement>
#include <QString>

XmlRpcRequest::XmlRpcRequest() {
    //_params = new QVariant();
}

XmlRpcRequest::XmlRpcRequest(QString name, QVariantList params) {
    setMethodName(name);
    setParams(params);
}

XmlRpcRequest *XmlRpcRequest::fromString(QByteArray data) {
    XmlRpcRequest *xmlrpc_request = new XmlRpcRequest();
    QDomDocument doc;
    if(doc.setContent(data)) {
        QDomElement methodCall = doc.documentElement();
        if(methodCall.isNull() || methodCall.tagName() != "methodCall") {
            return NULL;
        } else {
            for(QDomNode node = methodCall.firstChild(); !node.isNull(); node = node.nextSibling()) {
                if(!node.isElement()) continue;
                QDomElement element = node.toElement();
                if(element.tagName() == "methodName") {
                    xmlrpc_request->setMethodName(element.text());
                } else if(element.tagName() == "params") {
                    xmlrpc_request->setParams(element);
                }
            }
        }
    }
    return xmlrpc_request;
}

QString XmlRpcRequest::toString() {
    return "<methodCall><methodName>" + methodName() + "</methodName>" + toXmlRpcParams(params()) + "</methodCall>";
}

QString XmlRpcRequest::methodName() {
    return _methodname;
}

XmlRpcRequest *XmlRpcRequest::setMethodName(QString method_name) {
    _methodname = method_name;
    return this;
}

QVariantList XmlRpcRequest::params() {
    return _params;
}

XmlRpcRequest *XmlRpcRequest::setParams(const QDomElement &element) {
    return setParams(parseXmlRpcParams(element));
}

XmlRpcRequest *XmlRpcRequest::setParams(QVariantList params) {
    _params = params;
    return this;
}
