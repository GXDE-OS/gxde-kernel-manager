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

#include <QProcess>

#include <QFile>
#include <QMap>

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
    QString get_ver(int value) const;
    QStringList get_pkgName(int value) const;
    QStringList get_system(int value) const;
    QStringList get_arch(int value) const;
    bool get_installedAlready(int value) const;

    QString localKernelName() const;
    QString arch() const;

signals:
    void loadFinished(QNetworkReply *reply);



private:
    QString url = "http://info.kernel.gxde.gfdgdxi.top/information.json";
    QJsonArray listData;

    QStringList unShowMap = {
        "-dbg"
    };

    QStringList showMap = {
        "linux-headers",
        "linux-image",
        "linux-kernel"
    };

    QMap<QString, int> indexMap;


};

#endif // KERNELINFORMATION_H
