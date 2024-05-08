#include "aptpkginfo.h"
#include <QFile>
#include <QDir>
#include <QDebug>

AptPkgInfo::AptPkgInfo(QString pkgName, PkgSearchOption option)
{
    this->pkgSearchOption = option;
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
        if(i.mid(i.indexOf("_Packages")) != "_Packages") {
            continue;
        }
        QFile file(dir.path() + "/" + i);
        file.open(QFile::ReadOnly);
        // 分析
        QString pkgData;
        pkgDataStatus status = pkgDataStatus::None;
        while(!file.atEnd()) {
            QByteArray line = file.readLine();
            if(line.replace(" ", "").replace("\n", "") == "") {
                // 空行
                if(status == pkgDataStatus::IsContain) {
                    aptData += pkgData + "\n";
                }
                status = pkgDataStatus::EmptyLine;
                pkgData = "";  // 清空
                continue;
            }
            // 如果已经被检测为非要寻找的包名，则
            if(status == pkgDataStatus::UnContain) {
                continue;  // 忽略该行
            }
            // 分析行
            if(line.contains("Package: ")) {
                // 如果为包名行，则进行分析
                // 是否含有要求关键字
                QString pkgName = line.replace("Package: ", "").replace(" ", "").replace("\n", "");
                switch(this->pkgSearchOption) {
                case PkgSearchOption::Equal:
                    if(pkgName == this->pkgName) {
                        status = pkgDataStatus::IsContain;
                    }
                    else {
                        status = pkgDataStatus::UnContain;
                    }
                    break;
                case PkgSearchOption::Include:
                    if(pkgName.contains(this->pkgName)) {
                        status = pkgDataStatus::IsContain;
                    }
                    else {
                        status = pkgDataStatus::UnContain;
                    }
                    break;
                }
                pkgData += line + "\n";
                continue;
            }
            // 处理
            pkgData += line + "\n";
        }
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
