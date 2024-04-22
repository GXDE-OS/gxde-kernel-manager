#include "kernelinformation.h"

KernelInformation::KernelInformation()
{

}

void KernelInformation::LoadInfo()
{
    QUrl url(this->url);
    QUrlQuery query;
    query.addQueryItem("format", "j1");
    url.setQuery(query.toString(QUrl::FullyEncoded));
    qDebug() << url;
    QNetworkRequest request(url);
    QNetworkAccessManager *m_http = new QNetworkAccessManager(this);
    QNetworkReply *reply = m_http->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, m_http](){
        QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
        QByteArray data = reply->readAll();
        qDebug() << data;
        qDebug() << reply->error();
        this->listData = QJsonDocument::fromJson(data).array();
        emit loadFinished(reply);
    });
}

QJsonArray KernelInformation::get_listData() const
{
    return this->listData;
}

int KernelInformation::get_count() const
{
    return this->listData.count();
}

QJsonObject KernelInformation::get_kernelData(int value) const
{
    return this->listData.at(value).toObject();
}

QString KernelInformation::get_name(int value) const
{
    return get_kernelData(value).value("Name").toString();
}

QString KernelInformation::get_author(int value) const
{
    return get_kernelData(value).value("Author").toString();
}

QString KernelInformation::get_des(int value) const
{
    return get_kernelData(value).value("Des").toString().replace("\\n", "\n");
}

QStringList KernelInformation::get_pkgName(int value) const
{
    QJsonArray list = get_kernelData(value).value("PkgName").toArray();
    int count = list.count();
    QStringList result;
    for(int i = 0; i < count; i++) {
        result << list.at(i).toString();
    }
    return result;
}

QStringList KernelInformation::get_system(int value) const
{
    QJsonArray list = get_kernelData(value).value("System").toArray();
    int count = list.count();
    QStringList result;
    for(int i = 0; i < count; i++) {
        result << list.at(i).toString();
    }
    return result;
}

QStringList KernelInformation::get_arch(int value) const
{
    QJsonArray list = get_kernelData(value).value("Arch").toArray();
    int count = list.count();
    QStringList result;
    for(int i = 0; i < count; i++) {
        result << list.at(i).toString();
    }
    return result;
}
