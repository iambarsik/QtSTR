#include "f_test.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    F_TEST w;
    w.show();
    return a.exec();
}
