#include "login.h"
#include <QApplication>

//#include "simplecrypt.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Login l;
    l.show();

    return a.exec();
}
