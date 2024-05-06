#ifndef APTPKGINFO_H
#define APTPKGINFO_H

#include <QObject>
#include <QProcess>

class AptPkgInfo: QObject
{
    Q_OBJECT
public:
    AptPkgInfo(QString pkgName);

    QStringList GetAptPackageList(QString name);
    QByteArray GetCommandResult(QString command, QStringList argv, QProcessEnvironment env = QProcessEnvironment::systemEnvironment());
};

#endif // APTPKGINFO_H
