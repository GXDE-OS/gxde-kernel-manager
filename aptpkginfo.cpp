#include "aptpkginfo.h"

AptPkgInfo::AptPkgInfo()
{

}

QStringList AptPkgInfo::GetAptPackageList(QString name)
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

QByteArray AptPkgInfo::GetCommandResult(QString command, QStringList argv, QProcessEnvironment env)
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
