#include "manager.h"

ManagerQ::ManagerQ(CoreQ *core)
{
    this->core = core;
//<MODEL_CONSTRUCTOR>
/*<::db_generated::>*/ m_M_TEST = new M_TEST(core); modelList.push_back(m_M_TEST);
//<FORMAT_CONSTRUCTOR>
/*<::db_generated::>*/ formatList.push_back({1000, enum_F_TEST, "Тест формат НЭМ + СУБА", "НЭМ", "СУБА"});
/*<::db_generated::>*/ formatList.push_back({1001, enum_F_TESTFORM, "another Тест формат", "НЭМ", "СУБА"});
}

ManagerQ::~ManagerQ()
{
    foreach(auto m, modelList)  {
        m->disconnect();
    }
    modelList.clear();
//<MODEL_DESTRUCTOR>
/*<::db_generated::>*/ delete m_M_TEST;
//</MODEL_DESTRUCTOR>
}

FormatQ * ManagerQ::openFormat(STRformat_enum name)
{
    FormatQ *f = nullptr;
    switch(name)    {
//<FORMAT_SWITCH>
/*<::db_generated::>*/ case enum_F_TEST: f = new F_TEST(core); break;
/*<::db_generated::>*/ case enum_F_TESTFORM: f = new F_TESTFORM(core); break;
//</FORMAT_SWITCH>
        default:

        break;
    }
    return f;
}

