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
        while(q->next()) {
            uint com_id = q->value(0).toUInt();
            QString name = q->value(1).toString();
            QString model_name = q->value(2).toString();
            QString model_id = getCellFromTable("id","models","name",model_name);
            QString description = q->value(3).toString();

            COMMANDS.append(sDB_tag + QString(" const uint %1 = %2;").arg(name).arg(com_id));
            COMMANDS_CONSTRUCTOR.append(sDB_tag + QString(" dependings.push_back({ %1, %2 /*%3*/, \"%4\" });").arg(name).arg(model_id).arg(model_name).arg(description));
        }
    }

    buff_list.clear();
    buff_list = getNamesListFromTable("variables");

    for(int i = 0; i < buff_list.size(); i++) {
        QString sExecute = "SELECT id, name, model, type, size, description FROM variables WHERE name = '" + buff_list[i] + "'";
        QSharedPointer<QSqlQuery> q = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
        q->exec(sExecute);
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
                var = QString(" %1 core_%2[%3]; // %4").arg(type).arg(name).arg(size).arg(description);
                get_method = QString(" %1 get_%2(int position) { if(position < %3) return core_%2[position]; else return 0; }").arg(type).arg(name).arg(size);
                set_method = QString(" void set_%2(%1 value, int position) { if(position < %3) core_%2[position] = value; }").arg(type).arg(name).arg(size);
                if(type == "qint64")    {
                    var_constructor = QString(" for(int i = 0; i < %2; i++) { core_%1[i] = 0; }").arg(name).arg(size);
                    var_pack = QString(" for(int i = 0; i < %2; i++) { pushInt(core_%1[i]); }").arg(name).arg(size);
                    var_unpack = QString(" for(int i = 0; i < %2; i++) { popInt(arr,core_%1[i]); }").arg(name).arg(size);
                } else if(type == "double") {
                    var_constructor = QString(" for(int i = 0; i < %2; i++) { core_%1[i] = 0.0; }").arg(name).arg(size);
                    var_pack = QString(" for(int i = 0; i < %2; i++) { m_package.append(pushDouble(core_%1[i])); }").arg(name).arg(size);
                    var_unpack = QString(" for(int i = 0; i < %2; i++) { m_package.append(popDouble(arr,core_%1[i])); }").arg(name).arg(size);
                } else if(type == "char")   {
                    var_constructor = QString(" for(int i = 0; i < %2; i++) { core_%1[i] = 0; }").arg(name).arg(size);
                    var_pack = QString(" for(int i = 0; i < %2; i++) { pushChar(core_%1[i]); }").arg(name).arg(size);
                    var_unpack = QString(" for(int i = 0; i < %2; i++) { popChar(arr,core_%1[i]); }").arg(name).arg(size);
                }
            } else {
                var = QString(" %1 core_%2; // %3").arg(type).arg(name).arg(description);
                get_method = QString(" %1 get_%2() { return core_%2; }").arg(type).arg(name);
                set_method = QString(" void set_%2(%1 %2) { core_%2 = %2; }").arg(type).arg(name);
                if(type == "qint64")    {
                    var_constructor = QString(" core_%1 = 0;").arg(name);
                    var_pack = QString(" pushInt(core_%1);").arg(name);
                    var_unpack = QString(" popInt(arr,core_%1);").arg(name);
                } else if(type == "double") {
                    var_constructor = QString(" core_%1 = 0.0;").arg(name);
                    var_pack = QString(" m_package.append(pushDouble(core_%1));").arg(name);
                    var_unpack = QString(" popDouble(arr,core_%1);").arg(name);
                } else if(type == "char")   {
                    var_constructor = QString(" core_%1 = 0;").arg(name);
                    var_pack = QString(" pushChar(core_%1);").arg(name);
                    var_unpack = QString(" popChar(arr,core_%1);").arg(name);
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

void form::generateModels()
{
    QDir project_dir = QDir::currentPath() + "/" + sProjectDir;
    QDir models_dir = project_dir.path() + "/models";

    //qDebug() << project_dir.path();
    //qDebug() << models_dir.path();

    if(!project_dir.exists()) {
        qDebug() << "GEN :: project directory is not exists...";
        qDebug() << "GEN :: be sure that core generator had worked succesfuly...";
        return;
    }
    if(!models_dir.exists()) {
        qDebug() << "GEN :: models directory is not exists...";
        qDebug() << "GEN :: creating models directory...";
        models_dir.setPath(project_dir.path());
        if(!models_dir.mkdir("models")) {
                qDebug() << "GEN :: cant create models directory " << models_dir.path() + "/models";
                return;
        }
        models_dir.setPath(project_dir.path() + "/models");
        qDebug() << "GEN :: models directory in " << models_dir.path() << "is createed...";
    }

        // generating each model

    QStringList ModelList = getNamesListFromTable("models");

    foreach(QString mod, ModelList) {
        QDir current_model_dir = models_dir.path() + "/" + mod;

        if(!current_model_dir.exists()) {
            qDebug() << "GEN :: " << mod << " directory is not exists...";
            qDebug() << "GEN :: creating model directory...";
            current_model_dir.setPath(models_dir.path());
            if(!current_model_dir.mkdir(mod)) {
                    qDebug() << "GEN :: cant create " << current_model_dir.path();
                    return;
            }
            current_model_dir.setPath(models_dir.path() + "/" + mod);
            qDebug() << "GEN :: " << mod << " is createed...";
        }
        if(!QFile::exists(current_model_dir.path() + "/" + mod + ".pro"))   {
                // copy new model files from template
            if(
            !QFile::copy("../templates/model/m_template.pro",current_model_dir.path() + "/" + mod + ".pro") ||
            !QFile::copy("../templates/model/m_template.h",current_model_dir.path() + "/" + mod + ".h") ||
            !QFile::copy("../templates/model/m_template.cpp",current_model_dir.path() + "/" + mod + ".cpp")) {
                qDebug() << "GEN :: cant copy model files for " + mod + " from template";
                return;
            }
        }

        QFile fCode;
        QStringList originalCode;
        QStringList cleanCode;
        QStringList generatedCode;

        QString sTagName = "<GEN_MODEL_NAME>";

            // prepare correct data for fill code sections
        QStringList buff_list;
        QStringList COMMANDS;
        QStringList COMMANDS_DEFINES;
        QStringList COMMANDS_TO_SWITCH;
        QStringList VARS;
        QStringList VARS_FROM_CORE;
        QStringList VARS_TO_CORE;

        {
            QString sExecute = "SELECT id, name, par1_type, par2_type, par1_text, par2_text, model, description FROM commands WHERE model = '" + mod + "'";
            QSharedPointer<QSqlQuery> q = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
            q->exec(sExecute);
            while(q->next()) {
                uint com_id = q->value(0).toUInt();
                QString name = q->value(1).toString();
                QString par1_type = q->value(2).toString();
                QString par2_type = q->value(3).toString();
                QString par1_text = q->value(4).toString();
                QString par2_text = q->value(5).toString();
                QString model_name = q->value(6).toString();
                QString description = q->value(7).toString();

                COMMANDS.append(name);
                COMMANDS_DEFINES.append(sDB_tag + QString(" #define %1 %2 // %3").arg(name).arg(com_id).arg(description));
            }
        }

        {
            QString sExecute = "SELECT id, name, model, type, size, description FROM variables WHERE model = '" + mod + "'";
            QSharedPointer<QSqlQuery> q = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
            q->exec(sExecute);
            while(q->next()) {
                QString name = q->value(1).toString();
                QString type = q->value(3).toString();
                uint var_size = q->value(4).toUInt();
                QString size = QString("%1").arg(var_size);
                QString description = q->value(5).toString();

                QString var;
                QString var_from_core;
                QString var_to_core;

                if(var_size > 0)    {
                    var = QString(" %1 core_%2[%3]; // %4").arg(type).arg(name).arg(size).arg(description);
                    var_from_core = QString(" for(int i = 0; i < %2; i++) { %1[i] = core->get_%1(i); }").arg(name).arg(size);
                    var_to_core = QString(" for(int i = 0; i < %2; i++) { core->set_%1(%1[i],i); }").arg(name).arg(size);
                } else {
                    var = QString(" %1 core_%2; // %3").arg(type).arg(name).arg(description);
                    var_from_core = QString(" %1 = core->get_%1();").arg(name);
                    var_to_core = QString(" core->set_%1(%1);").arg(name);
                }

                VARS.append(sDB_tag + var);
                VARS_FROM_CORE.append(sDB_tag + var_from_core);
                VARS_TO_CORE.append(sDB_tag + var_to_core);
            }
        }

            // ==================================================== generating "model_name.pro" ===========================================================

        originalCode.clear();
        cleanCode.clear();
        generatedCode.clear();
        fCode.setFileName(current_model_dir.path() + "/" + mod + ".pro");
        if(fCode.open(QFile::ReadOnly)) {
            QTextStream out(&fCode);
            QString line;
            while (out.readLineInto(&line)) {
                originalCode.append(line);
                line.clear();
            }
        } else {
            qDebug() << "GEN :: cant open " << mod << ".pro file";
        }
        fCode.close();

        for(int i = 0; i < originalCode.size(); i++)    {
            if(originalCode[i].contains(sTagName))  {
               QString s = originalCode[i];
               s.replace(sTagName,mod);
               cleanCode.append(s);
               continue;
            }
            cleanCode.append(originalCode[i]);
        }

        for(int i = 0; i < cleanCode.size(); i++)    {
            generatedCode.append(cleanCode[i]);
            /*
            if(cleanCode[i].contains("//<VAR_SECTION>"))  {
                generatedCode.append(VARS);
            }
            */
        }

        saveCodeToFile(generatedCode,fCode);

            // ==================================================== generating "model_name.h" ===========================================================

        originalCode.clear();
        cleanCode.clear();
        generatedCode.clear();
        fCode.setFileName(current_model_dir.path() + "/" + mod + ".h");
        if(fCode.open(QFile::ReadOnly)) {
            QTextStream out(&fCode);
            QString line;
            while (out.readLineInto(&line)) {
                originalCode.append(line);
                line.clear();
            }
        } else {
            qDebug() << "GEN :: cant open " << mod << ".h file";
        }
        fCode.close();

        for(int i = 0; i < originalCode.size(); i++)    {
            if(originalCode[i].contains(sDB_tag))  {
                continue;
            }
            if(originalCode[i].contains(sTagName))  {
               QString s = originalCode[i];
               s.replace(sTagName,mod);
               cleanCode.append(s);
               continue;
            }
            cleanCode.append(originalCode[i]);
        }

        for(int i = 0; i < cleanCode.size(); i++)    {
            generatedCode.append(cleanCode[i]);
            if(cleanCode[i].contains("//<MODEL_ID_SECTION>"))  {
                generatedCode.append("#define MODEL_ID " + getCellFromTable("id","models","name",mod));
                generatedCode.append("QString MODEL_NAME = \"" + mod + "\"");
            }
            if(cleanCode[i].contains("//<DEFINE_COMMANDS_SECTION>"))  {
                generatedCode.append(COMMANDS_DEFINES);
            }
            if(cleanCode[i].contains("//<VAR_SECTION>"))  {
                generatedCode.append(VARS);
            }
        }

        saveCodeToFile(generatedCode,fCode);

            // ==================================================== generating "model_name.cpp" ===========================================================

        originalCode.clear();
        cleanCode.clear();
        generatedCode.clear();
        fCode.setFileName(current_model_dir.path() + "/" + mod + ".cpp");
        if(fCode.open(QFile::ReadOnly)) {
            QTextStream out(&fCode);
            QString line;
            while (out.readLineInto(&line)) {
                originalCode.append(line);
                line.clear();
            }
        } else {
            qDebug() << "GEN :: cant open " << mod << ".cpp file";
        }
        fCode.close();

        for(int i = 0; i < originalCode.size(); i++)    {
            if(originalCode[i].contains(sDB_tag))  {
                continue;
            }
            if(originalCode[i].contains(sTagName))  {
               QString s = originalCode[i];
               s.replace(sTagName,mod);
               cleanCode.append(s);
               continue;
            }
            cleanCode.append(originalCode[i]);
        }

        for(int i = 0; i < COMMANDS.size(); i++)    {
            bool bAdd = true;
            for(int c = 0; c < cleanCode.size(); c++)   {
                if(cleanCode[c].contains("case " + COMMANDS[i] + ":"))  {
                    bAdd = false;
                    break;
                }
            }
            if(bAdd == true)    {
                COMMANDS_TO_SWITCH.append(COMMANDS[i]);
            }
        }



        for(int i = 0; i < cleanCode.size(); i++)    {
            generatedCode.append(cleanCode[i]);
            if(cleanCode[i].contains("//<FROM_CORE_SECTION>"))  {
                generatedCode.append(VARS_FROM_CORE);
            }
            if(cleanCode[i].contains("//<TO_CORE_SECTION>"))  {
                generatedCode.append(VARS_TO_CORE);
            }
            if(cleanCode[i].contains("//<TO_CORE_METHOD>"))  {
                generatedCode.append(sDB_tag + " outputToCore();");
            }
            if(cleanCode[i].contains("//<COMMAND_SWITCH_TAG>"))  {
                for(int i = 0; i < COMMANDS_TO_SWITCH.size(); i++)  {
                    QString p1_type = getCellFromTable("par1_type", "commands", "name", COMMANDS_TO_SWITCH[i]);
                    QString p2_type = getCellFromTable("par2_type", "commands", "name", COMMANDS_TO_SWITCH[i]);
                    QString p1_text = getCellFromTable("par1_text", "commands", "name", COMMANDS_TO_SWITCH[i]);
                    QString p2_text = getCellFromTable("par2_text", "commands", "name", COMMANDS_TO_SWITCH[i]);
                    QString append1;
                    if(p1_type == "qint64") {
                        append1 = QString("\t\t\tqint64 p1 = command.par1.toInt(); // %1").arg(p1_text);
                    }
                    if(p1_type == "double") {
                        append1 = QString("\t\t\tdouble p1 = command.par1.toDouble(); // %1").arg(p1_text);
                    }
                    if(p1_type == "char") {
                        append1 = QString("\t\t\tchar p1 = command.par1.toChar(); // %1").arg(p1_text);
                    }
                    QString append2;
                    if(p2_type == "qint64") {
                        append2 = QString("\t\t\tqint64 p2 = command.par2.toInt(); // %1").arg(p2_text);
                    }
                    if(p2_type == "double") {
                        append2 = QString("\t\t\tdouble p2 = command.par2.toDouble(); // %1").arg(p2_text);
                    }
                    if(p2_type == "char") {
                        append2 = QString("\t\t\tchar p2 = command.par2.toChar(); // %1").arg(p2_text);
                    }

                    generatedCode.append("\t\tcase " + COMMANDS_TO_SWITCH[i] + ": { // " + getCellFromTable("description", "commands","name",COMMANDS_TO_SWITCH[i]));
                    generatedCode.append(append1);
                    generatedCode.append(append2);
                    generatedCode.append("\t\t\t// write code here");
                    generatedCode.append("");
                    generatedCode.append("\t\t\tQ_UNUSED(p1);");
                    generatedCode.append("\t\t\tQ_UNUSED(p2);");
                    generatedCode.append("\t\t} break;");
                }
            }
            /*
            if(cleanCode[i].contains("//<DEFINE_COMMANDS_SECTION>"))  {
                generatedCode.append(COMMANDS_DEFINES);
            }
            */
        }

        saveCodeToFile(generatedCode,fCode);






        /*
        for(int i = 0; i < generatedCode.size(); i++)    {
            qDebug() << generatedCode[i];
        }

        */


        //qDebug() << mod;
    }



}

void form::saveCodeToFile(const QStringList code, QFile &file)
{
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out.setCodec(QTextCodec::codecForName("UTF-8"));
    for(int i = 0; i < code.size(); i++)   {
        out << code[i] << "\n";
    }
    file.close();
}
