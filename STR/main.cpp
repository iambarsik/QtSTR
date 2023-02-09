#include "str.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    STR w;
    if(w.isMultiScreen())   {
        w.show();
    } else {
        w.showMaximized();
    }

    return a.exec();
}

