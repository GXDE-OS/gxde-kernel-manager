#ifndef KERNELINFORMATION_H
#define KERNELINFORMATION_H

#include <QString>

#include <QUrlQuery>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

class KernelInformation : public QObject
{
    Q_OBJECT

public:
    KernelInformation();
    void LoadInfo();
    QJsonArray get_listData() const;
    QJsonObject get_kernelData(int value) const;
    int get_count() const;
    QString get_name(int value) const;
    QString get_author(int value) const;
    QString get_des(int value) const;
    QStringList get_pkgName(int value) const;
    QStringList get_system(int value) const;
    QStringList get_arch(int value) const;

signals:
    void loadFinished(QNetworkReply *reply);


private:
    QString url = "http://127.0.0.1:8000/information.json";
    QJsonArray listData;
};

#endif // KERNELINFORMATION_H
