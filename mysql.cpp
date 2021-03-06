#include "mysql.h"

mysql::mysql()
{

}

QSqlDatabase mysql::connect_db(QString name)
{
    QFile db_path(PATH_SETTING_CONNECTION);
    if (db_path.exists()) {
        QSettings db_sett(PATH_SETTING_CONNECTION, QSettings::IniFormat);
        host = db_sett.value("HOST").toString();
        db_name = db_sett.value("DATABASE").toString();
        username = db_sett.value("USERNAME").toString();
        password = db_sett.value("PASSWORD").toString();

        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", name);
        db.setHostName(host);
        db.setDatabaseName(db_name);
        db.setUserName(username);
        db.setPassword(password);

        return (QSqlDatabase) db;
    } else {
        host = "localhost";
        db_name = "LSPE";
        username = "root";
        password = "root";

        QSettings db_sett(PATH_SETTING_CONNECTION, QSettings::IniFormat);
        db_sett.setValue("HOST", host.toUtf8());
        db_sett.setValue("DATABASE", db_name.toUtf8());
        db_sett.setValue("USERNAME", username.toUtf8());
        db_sett.setValue("PASSWORD", password.toUtf8());

        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", name);
        db.setHostName(host);
        db.setDatabaseName(db_name);
        db.setUserName(username);
        db.setPassword(password);

        return (QSqlDatabase) db;
    }
}

void mysql::close(QSqlDatabase db)
{
    QString connection;
    connection = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(connection);
}

QStringList mysql::read(QSqlDatabase db, QString query)
{
    QStringList result;
    if (db.isValid()) {
        if (!db.isOpen()) {
            db.open();
        }
        QSqlQuery q(QSqlDatabase::database(db.connectionName()));
//        qDebug() << query;
//        q.prepare(query);
        if (q.exec(query)) {
            while(q.next()) {
                for (int i = 0; i < q.record().count(); i++) {
                    result.append(q.value(i).toString());
                }
            }
        } else {
            qDebug() << "Cannot read from database with query: \n" << query;
//            qDebug() << "Cannot read from database";
        }
        db.close();
    }
    return result;
}

void mysql::write(QSqlDatabase db, QString query)
{
    if (db.isValid()) {
        if (!db.isOpen()) {
            db.open();
        }
        QSqlQuery q(QSqlDatabase::database(db.connectionName()));
//        qDebug() << query;
//        q.prepare(query);
        if (q.exec(query)) {
//            while(q.next()) {
//                 result.append(q.value(0).toString());
//            }
//            qDebug() << "Success query: \n" << query;
        } else {
            qDebug() << "Cannot write to database with query: \n" << query;
//            qDebug() << "Cannot write to database";
        }
        db.close();
    }
}
