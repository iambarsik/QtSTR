#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <QtCore>

struct model_struct {
    int id;
    QString name;
    QString object;
    QString system;
    QString description;
};

struct format_struct    {
    int id;
    QString name;
    QString title;
    QString object;
    QString system;
    QString description;
};

struct variable_struct  {
    int id;
    QString name;
    QString model;
    QString type;
    uint size;
    QString description;
};

struct command_struct   {
    int id;
    QString name;
    QString par1_type;
    QString par2_type;
    QString par1_text;
    QString par2_text;
    QString model;
    QString description;
};




#endif // DATA_STRUCT_H
