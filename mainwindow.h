#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

#include "mysql.h"
#include "form_pendaftaran.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, int user_id = 0, int user_type = 0);
    ~MainWindow();

private slots:
    void on_pb_admin_addNew_clicked();

    void on_pb_admin_delete_clicked();

    void on_pb_assesor_addNew_clicked();

    void on_pb_assesor_delete_clicked();

private:
    Ui::MainWindow *ui;

    mysql database;
    QSqlDatabase db;
    int id_user;
    int role_id;
    void setMainWindow();
    void setTabWindow();

    void get_admin();
    void get_sertifikat();
    void get_assesor();
};

#endif // MAINWINDOW_H
