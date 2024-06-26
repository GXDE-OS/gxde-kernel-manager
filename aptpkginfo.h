#ifndef APTPKGINFO_H
#define APTPKGINFO_H

#include <QObject>
#include <QProcess>
#include <QJsonObject>

class AptPkgInfo
{
public:
    enum PkgSearchOption {
        Include = 0,
        Equal = 1,
        HeadInclude = 2
    };

    explicit AptPkgInfo(QString pkgName, PkgSearchOption option = PkgSearchOption::Equal);

    void SetPkgName(QString pkgName);
    QStringList GetAptPackageList() const;
    QByteArray GetCommandResult(QString command, QStringList argv, QProcessEnvironment env = QProcessEnvironment::systemEnvironment()) const;

    QString GetPkgInfo(QString pkgName=NULL) const;

    QString get_package(QString pkgName) const;
    QString get_version(QString pkgName) const;
    QString get_maintainer(QString pkgName) const;
    QString get_description(QString pkgName) const;
    QString get_architecture(QString pkgName) const;

    QJsonObject get_data() const;

    bool CompareVersion(QString version1, QString version2) const;


private:
    QString pkgName;
    QString pkgInfo;
    QJsonObject aptData;
    PkgSearchOption pkgSearchOption = PkgSearchOption::Equal;
    QStringList symbolList = {"-", "~", "+"};
    enum SplitVersionStatus {
        Checking,
        MeetedEnglishLetter,
        MeetSymbol,
        Other
    };

    QStringList SplitVersion(QString version) const;

    enum pkgDataStatus {
        EmptyLine = 0,
        EmptyLineAgain = 1,
        Reading = 2,
        Readed = 3,
        None = 4,
        UnContain = 5,
        IsContain = 6,
        IsContainOldVersion = 7,
        IsContainNewVersion = 8,
        IsContainSameVersion = 9
    };

    void ReadAptData();
};

#endif // APTPKGINFO_H
