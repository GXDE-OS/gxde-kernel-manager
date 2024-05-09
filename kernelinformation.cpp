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
    AptPkgInfo info = AptPkgInfo("linux-", AptPkgInfo::PkgSearchOption::Include);
    QStringList list = info.GetAptPackageList();
    QJsonArray array;
    for(QString i: list) {
        QJsonObject object;
        info.SetPkgName(i);
        object.insert("Name", i);
        object.insert("Author", info.get_maintainer(i));
        object.insert("Des", info.get_maintainer(i));
        object.insert("Arch", info.get_architecture(i));
        array.append(object);
        qDebug() << object;
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
    QJsonArray list = get_kernelData(value).value("Arch").toArray();
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
