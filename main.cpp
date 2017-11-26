#include "login.h"
#include <QApplication>

//#include "simplecrypt.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    SimpleCrypt crypto(Q_UINT64_C(0x0c2ad4a4acb9f023));
//    QString my_password = "admin";
//    my_password = crypto.encryptToString(my_password);
//    my_password = crypto.decryptToString(my_password);

    Login l;
    l.show();

    return a.exec();
}
