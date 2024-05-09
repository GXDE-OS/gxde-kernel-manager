#include "aptpkginfo.h"
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QJsonObject>

AptPkgInfo::AptPkgInfo(QString pkgName, PkgSearchOption option)
{
    this->pkgSearchOption = option;
    SetPkgName(pkgName);
    ReadAptData();
}

void AptPkgInfo::ReadAptData()
{
    this->aptData = QJsonObject();
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
        QJsonObject pkgData;
        pkgDataStatus status = pkgDataStatus::None;
        QString strTemp;  // 因为直接 replace 会改变原来的值
        while(!file.atEnd()) {
            QByteArray line = file.readLine();
            strTemp = line;
            if(strTemp.replace(" ", "").replace("\n", "") == "") {
                // 空行
                if(status == pkgDataStatus::IsContain) {
                    aptData.insert(pkgData.value("Package").toString(), pkgData);
                }
                status = pkgDataStatus::EmptyLine;
                pkgData = QJsonObject();  // 清空
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
                strTemp = line;
                QString pkgName = strTemp.replace("Package: ", "").replace(" ", "").replace("\n", "");
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
                // 解析为 QJsonObject
                pkgData.insert("Package", pkgName);
                continue;
            }
            // 处理
            int index = line.indexOf(":");
            strTemp = line;
            strTemp.replace("\n", "");
            pkgData.insert(QString(strTemp.mid(0, index)), QString(strTemp.mid(index + 2)));
        }
        file.close();
    }
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

QString AptPkgInfo::get_package(QString pkgName) const
{
    return this->aptData.value(pkgName).toObject().value("Package").toString();
}

QString AptPkgInfo::get_version(QString pkgName) const
{
    return this->aptData.value(pkgName).toObject().value("Version").toString();
}

QString AptPkgInfo::get_maintainer(QString pkgName) const
{
    return this->aptData.value(pkgName).toObject().value("Maintainer").toString();
}

QString AptPkgInfo::get_description(QString pkgName) const
{
    return this->aptData.value(pkgName).toObject().value("Description").toString();
}

QString AptPkgInfo::get_architecture(QString pkgName) const
{
    return this->aptData.value(pkgName).toObject().value("Architecture").toString();
}

QStringList AptPkgInfo::GetAptPackageList() const
{
    return this->aptData.keys();
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
