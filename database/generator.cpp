#include "form.h"
#include "ui_form.h"

#include <QDir>
#include <QTextCodec>

void form::generateCore()
{
    QDir project_dir = QDir::currentPath() + "/" + sProjectDir;
    QDir core_dir = project_dir.path() + "/core";

    //log(project_dir.path());
    //log(core_dir.path());

    if(!project_dir.exists()) {
        log("GEN :: project directory is not exists");
        log("GEN :: creating project directory...");
        project_dir.setPath(QDir::currentPath());
        if(!project_dir.mkdir(sProjectDir)) {
                log(QString("GEN :: cant create project directory %1/%2").arg(project_dir.path()).arg(sProjectDir));
                return;
        }
        project_dir.setPath(QDir::currentPath() + "/" + sProjectDir);
        log(QString("GEN :: project in %1 is created").arg(project_dir.path() + "/" + sProjectDir));
    }
    if(!core_dir.exists()) {
        log("GEN :: core directory is not exists");
        log("GEN :: creating core directory...");
        core_dir.setPath(project_dir.path());
        if(!core_dir.mkdir("core")) {
                log(QString("GEN :: cant create core directory %1 /core").arg(core_dir.path()));
                return;
        }
        core_dir.setPath(project_dir.path() + "/core");
        log(QString("GEN :: core in %1 is created").arg(core_dir.path() + "/core"));
    }
    if(!QFile::exists(core_dir.path() + "/core.pro"))   {
            // copy new core files from template
        if(
        !QFile::copy("../templates/core/core.pro",core_dir.path() + "/core.pro") ||
        !QFile::copy("../templates/core/core.h",core_dir.path() + "/core.h") ||
        !QFile::copy("../templates/core/core.cpp",core_dir.path() + "/core.cpp") ||
        !QFile::copy("../templates/core/commandsq.h",core_dir.path() + "/commandsq.h") ||
        !QFile::copy("../templates/core/commandsq.cpp",core_dir.path() + "/commandsq.cpp")) {
            log("GEN :: cant copy core files from template");
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

        // push manualy system commands (problem with rus text when copy from template)
    COMMANDS_CONSTRUCTOR.append(QString(sDB_tag + " dependings.push_back({ STR_COMMAND_SET, 0, \"<:: Отработка НУ ::>\"});"));
    COMMANDS_CONSTRUCTOR.append(QString(sDB_tag + " dependings.push_back({ STR_COMMAND_START, 0, \"<:: Пуск динамики ::>\"});"));
    COMMANDS_CONSTRUCTOR.append(QString(sDB_tag + " dependings.push_back({ STR_COMMAND_STOP, 0, \"<:: Стоп динамики ::>\"});"));
    COMMANDS_CONSTRUCTOR.append(QString(sDB_tag + " dependings.push_back({ STR_COMMAND_END, 0, \"<:: Окончание тренировки ::>\"});"));

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
                    var_unpack = QString(" for(int i = 0; i < %2; i++) { popDouble(arr,core_%1[i]); }").arg(name).arg(size);
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
        log("GEN :: cant open core.h files");
        return;
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
        log("GEN :: cant open core.cpp files");
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
        log("GEN :: cant open commandsq.h files");
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
        log("GEN :: cant open commandsq.cpp files");
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

    log("GEN :: core is successfully generated");
}

void form::generateModels()
{
    QDir project_dir = QDir::currentPath() + "/" + sProjectDir;
    QDir models_dir = project_dir.path() + "/models";

    if(!project_dir.exists()) {
        log("GEN :: project directory is not exists");
        log("GEN :: be sure that core generator had worked succesfuly");
        return;
    }
    if(!models_dir.exists()) {
        log("GEN :: models directory is not exists");
        log("GEN :: creating models directory...");
        models_dir.setPath(project_dir.path());
        if(!models_dir.mkdir("models")) {
                log(QString("GEN :: cant create models directory %1/models").arg(models_dir.path()));
                return;
        }
        models_dir.setPath(project_dir.path() + "/models");
        log(QString("GEN :: models directory in %1 is created").arg(models_dir.path()));
    }

        // generating each model

    QStringList ModelList = getNamesListFromTable("models");

    foreach(QString mod, ModelList) {
        QDir current_model_dir = models_dir.path() + "/" + mod;

        if(!current_model_dir.exists()) {
            log(QString("GEN :: %1 directory is not exists").arg(mod));
            log("GEN :: creating model directory...");
            current_model_dir.setPath(models_dir.path());
            if(!current_model_dir.mkdir(mod)) {
                    log(QString("GEN :: cant create %1").arg(current_model_dir.path()));
                    return;
            }
            current_model_dir.setPath(models_dir.path() + "/" + mod);
            log(QString("GEN :: %1 is created").arg(mod));
        }
        if(!QFile::exists(current_model_dir.path() + "/" + mod + ".pro"))   {
                // copy new model files from template
            if(
            !QFile::copy("../templates/model/m_template.pro",current_model_dir.path() + "/" + mod + ".pro") ||
            !QFile::copy("../templates/model/m_template.h",current_model_dir.path() + "/" + mod + ".h") ||
            !QFile::copy("../templates/model/m_template.cpp",current_model_dir.path() + "/" + mod + ".cpp")) {
                log(QString("GEN :: cant copy model files for %1  from template").arg(mod));
                return;
            }
        }

        QFile fCode;
        QStringList originalCode;
        QStringList cleanCode;
        QStringList generatedCode;

        QString sTagName = "<GEN_MODEL_NAME>";
        QString sTagInfo = "<GEN_MODEL_INFO>";

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
                    var = QString(" %1 %2[%3]; // %4").arg(type).arg(name).arg(size).arg(description);
                    var_from_core = QString(" for(int i = 0; i < %2; i++) { %1[i] = core->get_%1(i); }").arg(name).arg(size);
                    var_to_core = QString(" for(int i = 0; i < %2; i++) { core->set_%1(%1[i],i); }").arg(name).arg(size);
                } else {
                    var = QString(" %1 %2; // %3").arg(type).arg(name).arg(description);
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
            log(QString("GEN :: cant open %1.pro file").arg(mod));
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
            log(QString("GEN :: cant open %1.h file").arg(mod));
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
                generatedCode.append(sDB_tag + " #define MODEL_ID " + getCellFromTable("id","models","name",mod));
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

            log(QString("GEN :: cant open %1.cpp file").arg(mod));
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
            if(originalCode[i].contains(sTagInfo))  {
               QString s = originalCode[i];
               QString obj = getCellFromTable("object","models","name",mod);
               QString sys = getCellFromTable("system","models","name",mod);
               s.replace(sTagInfo,QString("\"%1\",\"%2\",\"%3\"").arg(mod).arg(obj).arg(sys));
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
        }
        saveCodeToFile(generatedCode,fCode);
    }
    log("GEN :: models are successfully generated");
}

void form::generateFormats()
{
    QDir project_dir = QDir::currentPath() + "/" + sProjectDir;
    QDir formats_dir = project_dir.path() + "/formats";

    if(!project_dir.exists()) {
        log("GEN :: project directory is not exists");
        log("GEN :: be sure that core generator had worked succesfuly");
        return;
    }
    if(!formats_dir.exists()) {
        log("GEN :: formats directory is not exists");
        log("GEN :: creating formats directory...");
        formats_dir.setPath(project_dir.path());
        if(!formats_dir.mkdir("formats")) {
                log(QString("GEN :: cant create formats directory %1/formats").arg(formats_dir.path()));
                return;
        }
        formats_dir.setPath(project_dir.path() + "/formats");
        log(QString("GEN :: formats directory in %1 is createed").arg(formats_dir.path()));
    }

        // generating each format

    QStringList FormatList = getNamesListFromTable("formats");

    foreach(QString form, FormatList) {
        QDir current_format_dir = formats_dir.path() + "/" + form;

        if(!current_format_dir.exists()) {
            log(QString("GEN :: %1 directory is not exists").arg(form));
            log("GEN :: creating model directory...");
            current_format_dir.setPath(formats_dir.path());
            if(!current_format_dir.mkdir(form)) {
                    log(QString("GEN :: cant create %1").arg(current_format_dir.path()));
                    return;
            }
            current_format_dir.setPath(formats_dir.path() + "/" + form);
            log(QString("GEN :: %1 is createed").arg(form));
        }
        if(!QFile::exists(current_format_dir.path() + "/" + form + ".pro"))   {
                // copy new model files from template
            if(
            !QFile::copy("../templates/format/f_template.pro",current_format_dir.path() + "/" + form + ".pro") ||
            !QFile::copy("../templates/format/f_template.h",current_format_dir.path() + "/" + form + ".h") ||
            !QFile::copy("../templates/format/f_template.cpp",current_format_dir.path() + "/" + form + ".cpp") ||
            !QFile::copy("../templates/format/f_template.ui",current_format_dir.path() + "/" + form + ".ui")) {
                log(QString("GEN :: cant copy format files for %1  from template").arg(form));
                return;
            }
        }

        QFile fCode;
        QStringList originalCode;
        QStringList cleanCode;
        QStringList generatedCode;

        QString sTagName = "<GEN_FORMAT_NAME>";
        QString sTagTitle = "<GEN_FORMAT_TITLE>";
        QString sTagInfo = "<GEN_FORMAT_INFO>";
        QString sFormatTitle = getCellFromTable("title","formats","name",form);

            // ==================================================== generating "format_name.pro" ===========================================================

        originalCode.clear();
        cleanCode.clear();
        generatedCode.clear();
        fCode.setFileName(current_format_dir.path() + "/" + form + ".pro");
        if(fCode.open(QFile::ReadOnly)) {
            QTextStream out(&fCode);
            QString line;
            while (out.readLineInto(&line)) {
                originalCode.append(line);
                line.clear();
            }
        } else {
            log(QString("GEN :: cant open %1.pro file").arg(form));
        }
        fCode.close();

        for(int i = 0; i < originalCode.size(); i++)    {
            if(originalCode[i].contains(sTagName))  {
               QString s = originalCode[i];
               s.replace(sTagName,form);
               cleanCode.append(s);
               continue;
            }
            cleanCode.append(originalCode[i]);
        }

        /*
         * add something with generating if needs to be added
         * */

        for(int i = 0; i < cleanCode.size(); i++)    {
            generatedCode.append(cleanCode[i]);
            if(cleanCode[i].contains("//<FORMAT_ID_SECTION>"))  {
                generatedCode.append("#define FORMAT_ID " + getCellFromTable("id","formats","name",form));
            }
        }

        saveCodeToFile(generatedCode,fCode);

            // ==================================================== generating "format_name.h" ===========================================================

        originalCode.clear();
        cleanCode.clear();
        generatedCode.clear();
        fCode.setFileName(current_format_dir.path() + "/" + form + ".h");
        if(fCode.open(QFile::ReadOnly)) {
            QTextStream out(&fCode);
            QString line;
            while (out.readLineInto(&line)) {
                originalCode.append(line);
                line.clear();
            }
        } else {
            log(QString("GEN :: cant open %1.h file").arg(form));
        }
        fCode.close();

        for(int i = 0; i < originalCode.size(); i++)    {
            if(originalCode[i].contains(sDB_tag))  {
                continue;
            }
            if(originalCode[i].contains(sTagName))  {
               QString s = originalCode[i];
               s.replace(sTagName,form);
               cleanCode.append(s);
               continue;
            }
            cleanCode.append(originalCode[i]);
        }

        for(int i = 0; i < cleanCode.size(); i++)    {
            generatedCode.append(cleanCode[i]);
            if(cleanCode[i].contains("//<FORMAT_ID_SECTION>"))  {
                generatedCode.append(sDB_tag + " #define FORMAT_ID " + getCellFromTable("id","formats","name",form));
            }
        }

        saveCodeToFile(generatedCode,fCode);

            // ==================================================== generating "format_name.cpp" ===========================================================

        originalCode.clear();
        cleanCode.clear();
        generatedCode.clear();
        fCode.setFileName(current_format_dir.path() + "/" + form + ".cpp");
        if(fCode.open(QFile::ReadOnly)) {
            QTextStream out(&fCode);
            QString line;
            while (out.readLineInto(&line)) {
                originalCode.append(line);
                line.clear();
            }
        } else {
            log(QString("GEN :: cant open %1.cpp file").arg(form));
        }
        fCode.close();

        for(int i = 0; i < originalCode.size(); i++)    {
            if(originalCode[i].contains(sDB_tag))  {
                continue;
            }
            if(originalCode[i].contains(sTagName))  {
               QString s = originalCode[i];
               s.replace(sTagName,form);
               cleanCode.append(s);
               continue;
            }
            if(originalCode[i].contains(sTagInfo))  {
               QString s = originalCode[i];
               QString obj = getCellFromTable("object","formats","name",form);
               QString sys = getCellFromTable("system","formats","name",form);
               s.replace(sTagInfo,QString("\"%1\",\"%2\",\"%3\"").arg(form).arg(obj).arg(sys));
               cleanCode.append(s);
               continue;
            }
            if(originalCode[i].contains(sTagTitle))  {
                QString s = originalCode[i];
                s.replace(sTagTitle,sFormatTitle);
                cleanCode.append(s);
                continue;
            }
            cleanCode.append(originalCode[i]);
        }

        /*
         * add something with generating if needs to be added
         * */

        for(int i = 0; i < cleanCode.size(); i++)    {
            generatedCode.append(cleanCode[i]);
        }

        saveCodeToFile(generatedCode,fCode);

            // ==================================================== generating "format_name.ui" ===========================================================

        originalCode.clear();
        cleanCode.clear();
        generatedCode.clear();
        fCode.setFileName(current_format_dir.path() + "/" + form + ".ui");
        if(fCode.open(QFile::ReadOnly)) {
            QTextStream out(&fCode);
            QString line;
            while (out.readLineInto(&line)) {
                originalCode.append(line);
                line.clear();
            }
        } else {
            log(QString("GEN :: cant open %1.ui file").arg(form));
        }
        fCode.close();

        for(int i = 0; i < originalCode.size(); i++)    {
            if(originalCode[i].contains(sDB_tag))  {
                continue;
            }
            if(originalCode[i].contains(sTagName))  {
               QString s = originalCode[i];
               s.replace(sTagName,form);
               cleanCode.append(s);
               continue;
            }
            if(originalCode[i].contains(sTagTitle))  {
                QString s = originalCode[i];
                s.replace(sTagTitle,sFormatTitle);
                cleanCode.append(s);
                continue;
            }
            cleanCode.append(originalCode[i]);
        }

        /*
         * add something with generating if needs to be added
         * */

        for(int i = 0; i < cleanCode.size(); i++)    {
            generatedCode.append(cleanCode[i]);
        }

        saveCodeToFile(generatedCode,fCode);

    }
    log("GEN :: formats are successfully generated");
}

void form::generateManager()
{
    QDir project_dir = QDir::currentPath() + "/" + sProjectDir;
    QDir manager_dir = project_dir.path() + "/manager";

    if(!project_dir.exists()) {
        log("GEN :: project directory is not exists");
        log("GEN :: be sure that core generator had worked succesfuly");
        return;
    }
    if(!manager_dir.exists()) {
        log("GEN :: manager directory is not exists...");
        log("GEN :: creating manager directory...");
        manager_dir.setPath(project_dir.path());
        if(!manager_dir.mkdir("manager")) {
                log(QString("GEN :: cant create manager directory %1/manager").arg(manager_dir.path()));
                return;
        }
        manager_dir.setPath(project_dir.path() + "/manager");
        log(QString("GEN :: manager directory in %1 is createed...").arg(manager_dir.path()));
    }
    if(!QFile::exists(manager_dir.path() + "/manager.pro"))   {
            // copy new manager files from template
        if(
        !QFile::copy("../templates/manager/manager.pro",manager_dir.path() + "/manager.pro") ||
        !QFile::copy("../templates/manager/modules.pri",manager_dir.path() + "/modules.pri") ||
        !QFile::copy("../templates/manager/modules.h",manager_dir.path() + "/modules.h") ||
        !QFile::copy("../templates/manager/manager.h",manager_dir.path() + "/manager.h") ||
        !QFile::copy("../templates/manager/manager.cpp",manager_dir.path() + "/manager.cpp")) {
            log("GEN :: cant copy manager files from template");
            return;
        }
    }

    QFile fCode;
    QStringList originalCode;
    QStringList cleanCode;
    QStringList generatedCode;

    QStringList MODELS_H;
    QStringList M_MODULES_H;
    QStringList M_MODULES_PRI;
    QStringList MODELS_CONSTRUCTOR;
    QStringList MODELS_DESTRUCTOR;

        // prepare correct data for fill code sections
    QStringList buff_list;

    buff_list.clear();
    buff_list = getNamesListFromTable("models");

    for(int i = 0; i < buff_list.size(); i++) {
        MODELS_H.append(sDB_tag + QString(" %1 * m_%1;").arg(buff_list[i]));
        M_MODULES_H.append(sDB_tag + QString(" #include \"../models/%1/%1.h\"").arg(buff_list[i]));

        M_MODULES_PRI.append(QString("INCLUDEPATH += $$PWD/../models/%1").arg(buff_list[i]));
        M_MODULES_PRI.append(QString("DEPENDPATH += $$PWD/../models/%1").arg(buff_list[i]));
        M_MODULES_PRI.append(QString("win32: LIBS += -L$$PWD/../_output/ -l%1").arg(buff_list[i]));
        M_MODULES_PRI.append(QString(""));

        MODELS_CONSTRUCTOR.append(sDB_tag + QString(" m_%1 = new %1(core); modelList.push_back(m_%1);").arg(buff_list[i]));
        MODELS_DESTRUCTOR.append(sDB_tag + QString(" delete m_%1;").arg(buff_list[i]));
    }

    QStringList F_MODULES_H;
    QStringList FORMATS_ENUM;
    QStringList FORMATS_CONSTRUCTOR;
    QStringList FORMAT_SWITCH;
    QStringList F_MODULES_PRI;

    buff_list.clear();
    buff_list = getNamesListFromTable("formats");

    foreach(QString form, buff_list)    {
        QString sExecute = "SELECT id, title, object, system FROM formats WHERE name = '" + form + "'";
        QSharedPointer<QSqlQuery> q = QSharedPointer<QSqlQuery>(new QSqlQuery(db));
        q->exec(sExecute);
        while(q->next()) {

            uint form_id = q->value(0).toUInt();
            QString title = q->value(1).toString();
            QString object = q->value(2).toString();
            QString system = q->value(3).toString();

            F_MODULES_H.append(sDB_tag + QString(" #include \"../formats/%1/%1.h\"").arg(form));
            FORMATS_ENUM.append(sDB_tag + QString(" enum_%1,").arg(form));
            FORMATS_CONSTRUCTOR.append(sDB_tag + QString(" formatList.push_back({%2, enum_%1, \"%3\", \"%4\", \"%5\"});").arg(form).arg(form_id).arg(title).arg(object).arg(system));
            FORMAT_SWITCH.append(sDB_tag + QString(" case enum_%1: f = new %1(core); break;").arg(form));

            F_MODULES_PRI.append(QString("INCLUDEPATH += $$PWD/../formats/%1").arg(form));
            F_MODULES_PRI.append(QString("DEPENDPATH += $$PWD/../formats/%1").arg(form));
            F_MODULES_PRI.append(QString("win32: LIBS += -L$$PWD/../formats/ -l%1").arg(form));
            F_MODULES_PRI.append(QString(""));
        }
    }

    for(int i = 0; i < buff_list.size(); i++) {

    }

        // ==================================================== generating "manager.h" ===========================================================

    originalCode.clear();
    cleanCode.clear();
    generatedCode.clear();
    fCode.setFileName(manager_dir.path() + "/manager.h");
    if(fCode.open(QFile::ReadOnly)) {
        QTextStream out(&fCode);
        QString line;
        originalCode.clear();
        while (out.readLineInto(&line)) {
            originalCode.append(line);
            line.clear();
        }
    } else {
        log("GEN :: cant open manager.h files");
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
        if(cleanCode[i].contains("//<MODEL_SECTION>"))  {
            generatedCode.append(MODELS_H);
        }
    }

    saveCodeToFile(generatedCode,fCode);

        // ==================================================== generating "modules.h" ===========================================================

    originalCode.clear();
    cleanCode.clear();
    generatedCode.clear();
    fCode.setFileName(manager_dir.path() + "/modules.h");
    if(fCode.open(QFile::ReadOnly)) {
        QTextStream out(&fCode);
        QString line;
        originalCode.clear();
        while (out.readLineInto(&line)) {
            originalCode.append(line);
            line.clear();
        }
    } else {
        log("GEN :: cant open modules.h files");
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
        if(cleanCode[i].contains("//<M_MODULES_H>"))  {
            generatedCode.append(M_MODULES_H);
        }
        if(cleanCode[i].contains("//<F_MODULES_H>"))  {
            generatedCode.append(F_MODULES_H);
        }
        if(cleanCode[i].contains("//<ENUM_FORMAT>"))  {
            generatedCode.append(FORMATS_ENUM);
        }
    }

    saveCodeToFile(generatedCode,fCode);

        // ==================================================== generating "manager.cpp" ===========================================================

    originalCode.clear();
    cleanCode.clear();
    generatedCode.clear();
    fCode.setFileName(manager_dir.path() + "/manager.cpp");
    if(fCode.open(QFile::ReadOnly)) {
        QTextStream out(&fCode);
        QString line;
        originalCode.clear();
        while (out.readLineInto(&line)) {
            originalCode.append(line);
            line.clear();
        }
    } else {
        log("GEN :: cant open manager.cpp files");
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
        if(cleanCode[i].contains("//<MODEL_CONSTRUCTOR>"))  {
            generatedCode.append(MODELS_CONSTRUCTOR);
        }
        if(cleanCode[i].contains("//<FORMAT_CONSTRUCTOR>"))  {
            generatedCode.append(FORMATS_CONSTRUCTOR);
        }
        if(cleanCode[i].contains("//<MODEL_DESTRUCTOR>"))  {
            generatedCode.append(MODELS_DESTRUCTOR);
        }
        if(cleanCode[i].contains("//<FORMAT_SWITCH>"))  {
            generatedCode.append(FORMAT_SWITCH);
        }
    }

    saveCodeToFile(generatedCode,fCode);


        // ==================================================== generating "modules.pri" ===========================================================

    originalCode.clear();
    cleanCode.clear();
    generatedCode.clear();
    fCode.setFileName(manager_dir.path() + "/modules.pri");

    generatedCode.append("#--------------------------------- FORMATS ----------------------------------\n");
    generatedCode.append(F_MODULES_PRI);
    generatedCode.append("#--------------------------------- MODELS -----------------------------------\n");
    generatedCode.append(M_MODULES_PRI);

    saveCodeToFile(generatedCode,fCode);


    log("GEN :: module manager is successfully generated");
}

void form::generateGlobalProject()
{
    QDir project_dir = QDir::currentPath() + "/" + sProjectDir;
    if(!project_dir.exists()) {
        log("GEN :: project directory is not exists");
        log("GEN :: cant create global project file");
        return;
    }
    if(QFile::exists(project_dir.path() + "/GLOBAL_PROJECT.pro"))   {
        log("GEN :: generating new global project file...");
        if(!QFile::remove(project_dir.path() + "/GLOBAL_PROJECT.pro"))   {
                log("GEN :: cant remove old global project file");
                return;
        }
    }

    if(!QFile::copy("../templates/GLOBAL_PROJECT.pro",project_dir.path() + "/GLOBAL_PROJECT.pro"))    {
        log("GEN :: cant copy global project file from template");
        return;
    }

    QFile fCode;
    QStringList originalCode;
    QStringList cleanCode;
    QStringList generatedCode;

    QStringList buff_list;

    buff_list.clear();
    buff_list = getNamesListFromTable("models");

    QStringList MODELS;
    for(int i = 0; i < buff_list.size(); i++) {
        if(i < buff_list.size() - 1)
            MODELS.append("\t\tmodels/" + buff_list[i] + " \\");
        else
            MODELS.append("\t\tmodels/" + buff_list[i]);
    }

    buff_list.clear();
    buff_list = getNamesListFromTable("formats");

    QStringList FORMATS;
    for(int i = 0; i < buff_list.size(); i++) {
        if(i < buff_list.size() - 1)
            FORMATS.append("\t\tSUBDIRS += formats/" + buff_list[i] + " \\");
        else
            FORMATS.append("\t\tSUBDIRS += formats/" + buff_list[i]);
    }
    originalCode.clear();
    cleanCode.clear();
    generatedCode.clear();
    fCode.setFileName(project_dir.path() + "/GLOBAL_PROJECT.pro");
    if(fCode.open(QFile::ReadOnly)) {
        QTextStream out(&fCode);
        QString line;
        originalCode.clear();
        while (out.readLineInto(&line)) {
            originalCode.append(line);
            line.clear();
        }
    } else {
        log("GEN :: cant open GLOBAL_PROJECT.pro");
        return;
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
        if(cleanCode[i].contains("<MODELS>"))  {
            generatedCode.append(MODELS);
        }
        if(cleanCode[i].contains("<FORMATS>"))  {
            generatedCode.append(FORMATS);
        }
    }

    saveCodeToFile(generatedCode,fCode);

    log("GEN :: global project is successfully generated");
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
