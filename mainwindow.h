#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QLineEdit>
#include <QTableView>

#include "mysql.h"
#include "form_pendaftaran.h"
#include "form_peserta.h"

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

    void on_pb_assessor_addNew_clicked();
    void on_pb_assessor_delete_clicked();

    void on_tbl_admin_doubleClicked(const QModelIndex &index);
    void on_tbl_sertifikat_doubleClicked(const QModelIndex &index);
    void on_tbl_assessor_doubleClicked(const QModelIndex &index);

    void on_pb_sertifikat_addNew_clicked();
    void on_pb_sertifikat_delete_clicked();

    void on_pb_search_admin_clicked();
    void on_pb_search_sertifikat_clicked();
    void on_pb_search_assessor_clicked();

    void on_le_search_admin_returnPressed();
    void on_le_search_sertifikat_returnPressed();
    void on_le_search_assessor_returnPressed();

    void on_tbl_admin_clicked(const QModelIndex &index);
    void on_tbl_sertifikat_clicked(const QModelIndex &index);
    void on_tbl_assessor_clicked(const QModelIndex &index);

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
    void get_assessor();

    int search(int column, QLineEdit *le_search, QTableView *table_target);

    int row_focus_admin;
    int row_focus_sertifikat;
    int row_focus_assessor;

    void delete_admin(int id);
    void delete_sertifikat(int id);
    void delete_assessor(int id);
};

#endif // MAINWINDOW_H
