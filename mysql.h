#ifndef MYSQL_H
#define MYSQL_H

#include "utama.h"

class mysql
{
public:
    mysql();

    QString host;
    QString db_name;
    QString username;
    QString password;

    QSqlDatabase connect_db(QString name);
    void close(QSqlDatabase db);

    QStringList read(QSqlDatabase db, QString query);
    void write(QSqlDatabase db, QString query);
};

#endif // MYSQL_H
