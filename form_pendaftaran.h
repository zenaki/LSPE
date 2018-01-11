#ifndef FORM_PENDAFTARAN_H
#define FORM_PENDAFTARAN_H

#include <QDialog>
#include <QDateTime>
#include <QCryptographicHash>
#include <QFileDialog>
#include <QMessageBox>

#include "mysql.h"

namespace Ui {
class Form_Pendaftaran;
}

class Form_Pendaftaran : public QDialog
{
    Q_OBJECT

public:
    explicit Form_Pendaftaran(QWidget *parent = 0,
                              QSqlDatabase db_parent = QSqlDatabase(),
                              int mode = 0,
                              int user_id = 0,
                              bool edit = false,
                              int id = 0
            );
    ~Form_Pendaftaran();

private slots:
    void on_pb_admin_ok_clicked();
    void on_pb_admin_cancel_clicked();

    void on_pb_assessor_ok_clicked();
    void on_pb_assessor_cancel_clicked();

    void on_pb_sertifikat_ok_clicked();
    void on_pb_sertifikat_cancel_clicked();
    void on_pb_sertifikat_findPhoto_clicked();
    void on_pb_sertifikat_findCert_clicked();

private:
    Ui::Form_Pendaftaran *ui;

    mysql database;
    QSqlDatabase db;
    int id_user;
    int id_target;
    bool edit_mode;

    void setWindow(int mode);
    void setCompetence();
    QStringList competence;
    void setAssessor();
    QStringList assessor;
    void setEdit(bool edit, int id, int mode);
    void save_file(QString path, QString newName);

    bool validationAdminForm();
    bool validationassessorForm();
    bool validationSertifikatForm();
};

#endif // FORM_PENDAFTARAN_H
