#include "kernelinformation.h"
#include "aptpkginfo.h"

#include <QProcess>

KernelInformation::KernelInformation()
{

}

void KernelInformation::LoadInfo()
{
    // 从 apt 获取信息
    //QStringList data = this->GetAptPackageList("linux-*");
    /*for(QString i: data) {

    }*/
    AptPkgInfo info = AptPkgInfo("linux-", AptPkgInfo::PkgSearchOption::HeadInclude);
    QStringList list = info.GetAptPackageList();
    QJsonArray array;
    indexMap = {};
    for(QString i: list) {
        QJsonObject object;
        bool isContinue = false;
        for(QString j: unShowMap) {
            if(i.contains(j)) {
                isContinue = true;
                break;
            }
        }
        // 跳过此次循环
        if(isContinue) {
            continue;
        }
        isContinue = true;
        for(QString j: showMap) {
            if(i.contains(j)) {
                isContinue = false;
                break;
            }
        }
        // 跳过此次循环
        if(isContinue) {
            continue;
        }
        QString strTemp = i;
        strTemp.replace("-image", "");
        strTemp.replace("-headers", "");
        int alreadyIndex = -1;
        if(indexMap.contains(strTemp)) {
            // 如果已经存在
            alreadyIndex = indexMap.value(strTemp);
            QJsonArray pkgArray = array.at(alreadyIndex).toObject().value("PkgName").toArray();
            pkgArray.append(i);
            qDebug() << pkgArray;
            //array.replace(alreadyIndex, array.at(alreadyIndex).toObject().insert("PkgName", pkgArray)->toObject());
            qDebug() << alreadyIndex << array.count();
            QJsonObject pkgObject = array.at(alreadyIndex).toObject();
            pkgObject["PkgName"] = pkgArray;
            array.replace(alreadyIndex, pkgObject);//.insert("PkgName", pkgArray)->toObject());
            continue;
        }
        info.SetPkgName(strTemp);
        object.insert("Name", strTemp);
        object.insert("Author", info.get_maintainer(i));
        object.insert("Des", info.get_maintainer(i));
        object.insert("Arch", info.get_architecture(i));
        object.insert("PkgName", QJsonArray::fromStringList(QStringList() << i));
        indexMap.insert(strTemp, array.count());
        array.append(object);
    }
    this->listData = array;
    emit loadFinished(NULL);

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

QString KernelInformation::get_ver(int value) const
{
    return get_kernelData(value).value("Ver").toString();
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
    if(!result.count()) {
        result << "all";
    }
    return result;
}

QStringList KernelInformation::get_arch(int value) const
{
    //QJsonArray list = get_kernelData(value).value("Arch").toArray();
    /*int count = list.count();
    QStringList result;
    for(int i = 0; i < count; i++) {
        result << list.at(i).toString();
    }
    if(!result.count()) {
        result << "all";
    }*/
    return QStringList() << get_kernelData(value).value("Arch").toString();
}

QString KernelInformation::localKernelName() const
{
    QProcess process;
    process.start("uname", QStringList() << "-r");
    process.waitForStarted();
    process.waitForFinished();
    QString result = process.readAllStandardOutput().replace("\n", "").replace(" ", "");
    process.close();
    return result;
}

bool KernelInformation::get_installedAlready(int value) const
{
    QString pkgName = this->get_pkgName(value).at(0);
    return QFile::exists("/var/lib/dpkg/info/" + pkgName + ".list");
}

QString KernelInformation::arch() const
{
    QProcess process;
    process.start("dpkg", QStringList() << "--print-architecture");
    process.waitForStarted();
    process.waitForFinished();
    QString data = process.readAllStandardOutput();
    data = data.replace("\n", "").replace(" ", "");
    process.close();
    return data;
}
