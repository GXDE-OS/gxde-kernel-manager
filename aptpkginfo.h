#ifndef APTPKGINFO_H
#define APTPKGINFO_H

#include <QObject>
#include <QProcess>

class AptPkgInfo: QObject
{
    Q_OBJECT
public:
    enum PkgSearchOption {
        Include = 0,
        Equal = 1
    };

    AptPkgInfo(QString pkgName, PkgSearchOption option = PkgSearchOption::Equal);

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
    PkgSearchOption pkgSearchOption = PkgSearchOption::Equal;

    enum pkgDataStatus {
        EmptyLine = 0,
        EmptyLineAgain = 1,
        Reading = 2,
        Readed = 3,
        None = 4,
        UnContain = 5,
        IsContain = 6
    };

    void ReadAptData();
};

#endif // APTPKGINFO_H
