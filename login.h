#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QCryptographicHash>
#include <QMessageBox>

#include "mysql.h"

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void on_pb_login_clicked();

private:
    Ui::Login *ui;

    mysql database;
    QSqlDatabase db;
};

#endif // LOGIN_H
