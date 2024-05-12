#ifndef PROGRAMINFO_H
#define PROGRAMINFO_H

#include <QString>

namespace ProgramInfo
{
    enum LANG {
        zh_CN = 0,
        en_US = 1,
        other = en_US
    };
    LANG systemLANG() ;
    QString updateInfo();
    QString version();
};

#endif // PROGRAMINFO_H
