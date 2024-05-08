#ifndef APTPKGINFO_H
#define APTPKGINFO_H

#include <QObject>
#include <QProcess>

class AptPkgInfo: QObject
{
    Q_OBJECT
public:
    AptPkgInfo(QString pkgName);

    void SetPkgName(QString pkgName);
    QStringList GetAptPackageList(QString name) const;
    QByteArray GetCommandResult(QString command, QStringList argv, QProcessEnvironment env = QProcessEnvironment::systemEnvironment()) const;

    QString GetPkgInfo(QString pkgName=NULL) const;

    QString get_package() const;
    QString get_version() const;
    QString get_maintainer() const;

private:
    QString pkgName;
    QString pkgInfo;
    QString aptData;

    void ReadAptData();
};

#endif // APTPKGINFO_H
