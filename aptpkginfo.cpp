#include "aptpkginfo.h"
#include <QFile>
#include <QDir>
#include <QDebug>

AptPkgInfo::AptPkgInfo(QString pkgName)
{
    SetPkgName(pkgName);
    ReadAptData();
}

void AptPkgInfo::ReadAptData()
{
    this->aptData = "";
    QDir dir("/var/lib/apt/lists/");
    QStringList list = dir.entryList();
    for(QString i: list) {
        // 除去 . 和 ..
        if(i == "." || i == "..") {
            continue;
        }
        // TODO: bug
        if(i.mid(-10, -1) != "_Packages") {
            continue;
        }
        QFile file(dir.path() + "/" + i);
        file.open(QFile::ReadOnly);
        aptData += file.readAll() + "\n";
        file.close();
    }
    qDebug() << aptData;
}

void AptPkgInfo::SetPkgName(QString pkgName)
{
    this->pkgName = pkgName;
    pkgInfo = GetPkgInfo(pkgName);
}

QString AptPkgInfo::GetPkgInfo(QString pkgName) const
{
    if(pkgName == NULL) {
        pkgName = this->pkgName;
    }
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LANG", "en");
    return this->GetCommandResult("apt", QStringList() << "list" << pkgName, env);
}

QString AptPkgInfo::get_package() const
{
    QStringList list = pkgInfo.split("\n");
    for(QString i: list) {
        if(i.contains("Package: ")) {
            return i.replace("Package: ", "").replace(" ", "");
        }
    }
    return NULL;
}

QString AptPkgInfo::get_version() const
{
    QStringList list = pkgInfo.split("\n");
    for(QString i: list) {
        if(i.contains("Maintainer: ")) {
            return i.replace("Maintainer: ", "").replace(" ", "");
        }
    }
    return NULL;
}

QString AptPkgInfo::get_maintainer() const
{
    QStringList list = pkgInfo.split("\n");
    for(QString i: list) {
        if(i.contains("Version: ")) {
            return i.replace("Version: ", "").replace(" ", "");
        }
    }
    return NULL;
}

QStringList AptPkgInfo::GetAptPackageList(QString name) const
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("LANG", "en");
    QString data = GetCommandResult("apt", QStringList() << "list" << name, env);
    QStringList lineData = data.split("\n");
    QStringList result = {};
    for(QString i: lineData) {
        if(i.contains("Listing...")) {
            continue;
        }
        result.append(i.split("/").at(0));
    }
    return result;
}

QByteArray AptPkgInfo::GetCommandResult(QString command, QStringList argv, QProcessEnvironment env) const
{
    QProcess process;
    process.setProcessEnvironment(env);
    process.start(command, argv);
    process.waitForStarted();
    process.waitForFinished();
    QByteArray result = process.readAllStandardOutput();
    process.close();
    return result;
}
