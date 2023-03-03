#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QList>

#include "../include/common.h"
#include "modules.h"

class Q_DECL_EXPORT ManagerQ : public QObject
{
    Q_OBJECT

public:
    ManagerQ(CoreQ *core);
    ~ManagerQ();

    QList <ModelQ *> modelList;

    FormatQ * openFormat(STRformat_enum name);
    QList<STRFormat<STRformat_enum>> getFormatList() { return formatList; }


private:
    CoreQ *core;
    QList<STRFormat<STRformat_enum>> formatList;

//<MODEL_SECTION>
/*<::db_generated::>*/ M_TEST *m_M_TEST;
//</MODEL_SECTION>

//<FORMAT_SECTION>
/*<::db_generated::>*/ //F_TEST *f_F_TEST;
//</FORMAT_SECTION>

};

#endif // MANAGER_H
