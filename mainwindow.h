#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QStandardItemModel>

#include "mysql.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, int user_type = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    mysql database;
    QSqlDatabase db;
    int role_id;
    void setMainWindow();
    void setTabWindow();

    void get_admin();
    void get_sertifikat();
    void get_assesor();
};

#endif // MAINWINDOW_H
