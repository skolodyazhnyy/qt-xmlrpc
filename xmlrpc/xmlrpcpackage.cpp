#include "xmlrpcpackage.h"

XmlRpcPackage::XmlRpcPackage() { }

XmlRpcPackage::XmlRpcPackage(QString name) {
    setName(name);
}

XmlRpcPackage::XmlRpcPackage(QString name, QVariantList params) {
    setName(name);
    setParams(params);
}

XmlRpcPackage *XmlRpcPackage::fromString(QByteArray data) {
    XmlRpcPackage *package = new XmlRpcPackage();
    QDomDocument doc;
    if(doc.setContent(data)) {
        QDomElement root = doc.documentElement();
        if(root.isNull() || root.tagName() != "package") {
            return NULL;
        } else {
            for(QDomNode node = root.firstChild(); !node.isNull(); node = node.nextSibling()) {
                if(!node.isElement()) continue;
                QDomElement element = node.toElement();
                if(element.tagName() == "name") {
                    package->setName(element.text());
                } else if(element.tagName() == "params") {
                    package->setParams(element);
                }
            }
        }
    }
    return package;
}

QString XmlRpcPackage::toString() {
    return "<package><name>" + name() + "</name>" + toXmlRpcParams(params()) + "</package>";
}

QString XmlRpcPackage::name() {
    return _name;
}

XmlRpcPackage *XmlRpcPackage::setName(QString name) {
    _name = name;
    return this;
}

QVariantList XmlRpcPackage::params() {
    return _params;
}

XmlRpcPackage *XmlRpcPackage::setParams(const QDomElement &element) {
    return setParams(parseXmlRpcParams(element));
}

XmlRpcPackage *XmlRpcPackage::setParams(QVariantList params) {
    _params = params;
    return this;
}

XmlRpcPackage *XmlRpcPackage::addParam(QVariant param) {
    _params.append(param);
    return this;
}

QByteArray XmlRpcPackage::toByteArray() {
    return this->toString().toLatin1();
}
