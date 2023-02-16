#include "form.h"
#include "ui_form.h"

#include <QDir>
#include <QTextCodec>

void form::generateCore()
{
    QDir project_dir = QDir::currentPath() + "/" + sProjectDir;
    QDir core_dir = project_dir.path() + "/core";

    qDebug() << project_dir.path();
    qDebug() << core_dir.path();

    if(!project_dir.exists()) {
        qDebug() << "GEN :: project directory is not exists...";
        qDebug() << "GEN :: creating project directory...";
        project_dir.setPath(QDir::currentPath());
        if(!project_dir.mkdir(sProjectDir)) {
                qDebug() << "GEN :: cant create project directory " << project_dir.path() + "/" + sProjectDir;
                return;
        }
        project_dir.setPath(QDir::currentPath() + "/" + sProjectDir);
        qDebug() << "GEN :: project in " << project_dir.path() + "/" + sProjectDir << "is createed...";
    }
    if(!core_dir.exists()) {
        qDebug() << "GEN :: core directory is not exists...";
        qDebug() << "GEN :: creating core directory...";
        core_dir.setPath(project_dir.path());
        if(!core_dir.mkdir("core")) {
                qDebug() << "GEN :: cant create core directory " << core_dir.path() + "/core";
                return;
        }
        core_dir.setPath(project_dir.path() + "/core");
        qDebug() << "GEN :: core in " << core_dir.path() + "/core" << "is createed...";
    }
    if(!QFile::exists(core_dir.path() + "/core.pro"))   {
            // copy new core files from template
        if(
        !QFile::copy("../templates/core/core.pro",core_dir.path() + "/core.pro") ||
        !QFile::copy("../templates/core/core.h",core_dir.path() + "/core.h") ||
        !QFile::copy("../templates/core/core.cpp",core_dir.path() + "/core.cpp") ||
        !QFile::copy("../templates/core/commandsq.h",core_dir.path() + "/commandsq.h") ||
        !QFile::copy("../templates/core/commandsq.cpp",core_dir.path() + "/commandsq.cpp")) {
            qDebug() << "GEN :: cant copy core files from template";
            return;
        }
    }

    QFile fCode;
    QStringList originalCode;
    QStringList cleanCode;
    QStringList generatedCode;

        // prepare correct data for fill code sections
    QStringList buff_list;
    QStringList VARS_GET;
    QStringList VARS_SET;
    QStringList VARS;
    QStringList VARS_CONSTRUCTOR;
    QStringList VARS_PACK;
    QStringList VARS_UNPACK;

    buff_list.clear();
    buff_list = getNamesListFromTable("models");

    QStringList MODELS;
    for(int i = 0; i < buff_list.size(); i++) {
        MODELS.append(sDB_tag + " friend class " + buff_list[i] + ";" );
    }

    QStringList COMMANDS;
    QStringList COMMANDS_CONSTRUCTOR;

    buff_list.clear();
    buff_list = getNamesListFromTable("commands");
    for(int i = 0; i < buff_list.size(); i++) {
        QString sExecute = "SELECT id, name, model, description FROM commands WHERE name = '" + buff_list[i] + "'";
        QSharedPointer<QSqlQuery> q = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
        q->exec(sExecute);
        QSqlRecord r = q->record();
        while(q->next()) {
            uint com_id = q->value(0).toUInt();
            QString name = q->value(1).toString();
            QString model_name = q->value(2).toString();
            QString model_id = getCellFromTable("id","models","name",model_name);
            QString description = q->value(3).toString();

            COMMANDS.append(sDB_tag + QString(" const uint %1 = %2;").arg(name).arg(com_id));
            COMMANDS_CONSTRUCTOR.append(sDB_tag + QString("dependings.push_back({ %1, %2 /*%3*/, \"%4\" });").arg(name).arg(model_id).arg(model_name).arg(description));
        }
    }

    buff_list.clear();
    buff_list = getNamesListFromTable("variables");

    for(int i = 0; i < buff_list.size(); i++) {
        QString sExecute = "SELECT id, name, model, type, size, description FROM variables WHERE name = '" + buff_list[i] + "'";
        QSharedPointer<QSqlQuery> q = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
        q->exec(sExecute);
        QSqlRecord r = q->record();
        while(q->next()) {
            QString name = q->value(1).toString();
            QString type = q->value(3).toString();
            uint var_size = q->value(4).toUInt();
            QString size = QString("%1").arg(var_size);
            QString description = q->value(5).toString();

            QString get_method;
            QString set_method;
            QString var_constructor;
            QString var;
            QString var_pack;
            QString var_unpack;

            if(var_size > 0)    {
                var = QString("%1 core_%2[%3]; // %4").arg(type).arg(name).arg(size).arg(description);
                get_method = QString("%1 get_%2(int position) { if(position < %3) return core_%2[position]; else return 0; }").arg(type).arg(name).arg(size);
                set_method = QString("void set_%2(%1 value, int position) { if(position < %3) core_%2[position] = value; }").arg(type).arg(name).arg(size);
                if(type == "qint32")    {
                    var_constructor = QString("for(int i = 0; i < %2; i++) { core_%1[i] = 0; }").arg(name).arg(size);
                    var_pack = QString("for(int i = 0; i < %2; i++) { pushInt(core_%1[i]); }").arg(name).arg(size);
                    var_unpack = QString("for(int i = 0; i < %2; i++) { popInt(arr,core_%1[i]); }").arg(name).arg(size);
                } else if(type == "double") {
                    var_constructor = QString("for(int i = 0; i < %2; i++) { core_%1[i] = 0.0f; }").arg(name).arg(size);
                    var_pack = QString("for(int i = 0; i < %2; i++) { m_package.append(pushDouble(core_%1[i])); }").arg(name).arg(size);
                    var_unpack = QString("for(int i = 0; i < %2; i++) { m_package.append(popDouble(arr,core_%1[i])); }").arg(name).arg(size);
                } else if(type == "char")   {
                    var_constructor = QString("for(int i = 0; i < %2; i++) { core_%1[i] = 0; }").arg(name).arg(size);
                    var_pack = QString("for(int i = 0; i < %2; i++) { pushChar(core_%1[i]); }").arg(name).arg(size);
                    var_unpack = QString("for(int i = 0; i < %2; i++) { popChar(arr,core_%1[i]); }").arg(name).arg(size);
                }
            } else {
                var = QString("%1 core_%2; // %3").arg(type).arg(name).arg(description);
                get_method = QString("%1 get_%2() { return core_%2; }").arg(type).arg(name);
                set_method = QString("void set_%2(%1 %2) { core_%2 = %2; }").arg(type).arg(name);
                if(type == "qint32")    {
                    var_constructor = QString("core_%1 = 0;").arg(name);
                    var_pack = QString("pushInt(core_%1);").arg(name);
                    var_unpack = QString("popInt(arr,core_%1);").arg(name);
                } else if(type == "double") {
                    var_constructor = QString("core_%1 = 0.0f;").arg(name);
                    var_pack = QString("m_package.append(pushDouble(core_%1));").arg(name);
                    var_unpack = QString("popDouble(arr,core_%1);").arg(name);
                } else if(type == "char")   {
                    var_constructor = QString("core_%1 = 0;").arg(name);
                    var_pack = QString("pushChar(core_%1);").arg(name);
                    var_unpack = QString("popChar(arr,core_%1);").arg(name);
                }
            }

            VARS.append(sDB_tag + var);
            VARS_GET.append(sDB_tag + get_method);
            VARS_SET.append(sDB_tag + set_method);

            VARS_CONSTRUCTOR.append(sDB_tag + var_constructor);
            VARS_PACK.append(sDB_tag + var_pack);
            VARS_UNPACK.append(sDB_tag + var_unpack);

        }
    }

        // ==================================================== generating "core.h" ===========================================================

    originalCode.clear();
    cleanCode.clear();
    generatedCode.clear();
    fCode.setFileName(core_dir.path() + "/core.h");
    if(fCode.open(QFile::ReadOnly)) {
        QTextStream out(&fCode);
        QString line;
        originalCode.clear();
        while (out.readLineInto(&line)) {
            originalCode.append(line);
            line.clear();
        }
    } else {
        qDebug() << "GEN :: cant open core.h files";
    }
    fCode.close();

    for(int i = 0; i < originalCode.size(); i++)    {
        if(originalCode[i].contains(sDB_tag))  {
            continue;
        }
        cleanCode.append(originalCode[i]);
    }

    for(int i = 0; i < cleanCode.size(); i++)    {
        generatedCode.append(cleanCode[i]);
        if(cleanCode[i].contains("//<FRIEND_MODEL_SECTION>"))  {
            generatedCode.append(MODELS);
        }
        if(cleanCode[i].contains("//<GET_SECTION>"))  {
            generatedCode.append(VARS_GET);
        }
        if(cleanCode[i].contains("//<SET_SECTION>"))  {
            generatedCode.append(VARS_SET);
        }
        if(cleanCode[i].contains("//<VAR_SECTION>"))  {
            generatedCode.append(VARS);
        }
    }

    saveCodeToFile(generatedCode,fCode);

        // ==================================================== generating "core.cpp" ===========================================================

    originalCode.clear();
    cleanCode.clear();
    generatedCode.clear();
    fCode.setFileName(core_dir.path() + "/core.cpp");
    if(fCode.open(QFile::ReadOnly)) {
        QTextStream out(&fCode);
        QString line;
        originalCode.clear();
        while (out.readLineInto(&line)) {
            originalCode.append(line);
            line.clear();
        }
    } else {
        qDebug() << "GEN :: cant open core.cpp files";
    }
    fCode.close();

    for(int i = 0; i < originalCode.size(); i++)    {
        if(originalCode[i].contains(sDB_tag))  {
            continue;
        }
        cleanCode.append(originalCode[i]);
    }

    for(int i = 0; i < cleanCode.size(); i++)    {
        generatedCode.append(cleanCode[i]);
        if(cleanCode[i].contains("//<CONSTRUCTOR_SECTION>"))  {
            generatedCode.append(VARS_CONSTRUCTOR);
        }
        if(cleanCode[i].contains("//<PACK_SECTION>"))  {
            generatedCode.append(VARS_PACK);
        }
        if(cleanCode[i].contains("//<UNPACK_SECTION>"))  {
            generatedCode.append(VARS_UNPACK);
        }
    }

    saveCodeToFile(generatedCode,fCode);

        // ==================================================== generating "commandsq.h" ===========================================================

    originalCode.clear();
    cleanCode.clear();
    generatedCode.clear();
    fCode.setFileName(core_dir.path() + "/commandsq.h");
    if(fCode.open(QFile::ReadOnly)) {
        QTextStream out(&fCode);
        QString line;
        originalCode.clear();
        while (out.readLineInto(&line)) {
            originalCode.append(line);
            line.clear();
        }
    } else {
        qDebug() << "GEN :: cant open commandsq.h files";
    }
    fCode.close();

    for(int i = 0; i < originalCode.size(); i++)    {
        if(originalCode[i].contains(sDB_tag))  {
            continue;
        }
        cleanCode.append(originalCode[i]);
    }

    for(int i = 0; i < cleanCode.size(); i++)    {
        generatedCode.append(cleanCode[i]);
        if(cleanCode[i].contains("//<COMMANDS_SECTION>"))  {
            generatedCode.append(COMMANDS);
        }
    }

    saveCodeToFile(generatedCode,fCode);

        // ==================================================== generating "commandsq.cpp" ===========================================================

    originalCode.clear();
    cleanCode.clear();
    generatedCode.clear();
    fCode.setFileName(core_dir.path() + "/commandsq.cpp");
    if(fCode.open(QFile::ReadOnly)) {
        QTextStream out(&fCode);
        QString line;
        originalCode.clear();
        while (out.readLineInto(&line)) {
            originalCode.append(line);
            line.clear();
        }
    } else {
        qDebug() << "GEN :: cant open commandsq.cpp files";
    }
    fCode.close();

    for(int i = 0; i < originalCode.size(); i++)    {
        if(originalCode[i].contains(sDB_tag))  {
            continue;
        }
        cleanCode.append(originalCode[i]);
    }

    for(int i = 0; i < cleanCode.size(); i++)    {
        generatedCode.append(cleanCode[i]);
        if(cleanCode[i].contains("//<CONSTRUCTOR_SECTION>"))  {
            generatedCode.append(COMMANDS_CONSTRUCTOR);
        }
    }

    saveCodeToFile(generatedCode,fCode);

    /*
        for(int i = 0; i < generatedCode.size(); i++)    {
            qDebug() << generatedCode[i];
        }
        for(int i = 0; i < MODELS.size(); i++)    {
            qDebug() << MODELS[i];
        }
        for(int i = 0; i < VARS_GET.size(); i++)    {
            qDebug() << VARS_GET[i];
        }
        for(int i = 0; i < VARS.size(); i++)    {
            qDebug() << VARS[i];
        }
        for(int i = 0; i < VARS_SET.size(); i++)    {
            qDebug() << VARS_SET[i];
        }
    */
}

void form::saveCodeToFile(const QStringList code, QFile &file)
{
    file.open(QIODevice::ReadWrite);
    QTextStream out(&file);
    out.setCodec(QTextCodec::codecForName("UTF-8"));
    for(int i = 0; i < code.size(); i++)   {
        out << code[i] << "\n";
    }
    file.close();
}
