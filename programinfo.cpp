#include "programinfo.h"

#include <QFile>
#include <QProcessEnvironment>

ProgramInfo::LANG ProgramInfo::systemLANG()
{
    if(QProcessEnvironment::systemEnvironment().value("LANG").contains("zh")) {
        return LANG::zh_CN;
    }
    return LANG::en_US;
}


QString ProgramInfo::updateInfo()
{
    // 读取更新日志
    // 检查系统语言以加载对应语言的更新日志
    QString updateInfoPath = ":/data/new/en_US.html";
    if(QProcessEnvironment::systemEnvironment().value("LANG").contains("zh")) {
        // 中文，加载中文更新日志
        updateInfoPath = ":/data/new/zh_CN.html";
    }
    QFile file(updateInfoPath);
    file.open(QFile::ReadOnly);
    QString data = file.readAll();
    file.close();
    return data;
}

QString ProgramInfo::version()
{
    QFile file(":/data/version");
    file.open(QFile::ReadOnly);
    QString data = file.readAll();
    file.close();
    return data;
}
