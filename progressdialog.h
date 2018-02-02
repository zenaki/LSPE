#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>

#include <QDebug>
#include <QObject>
#include <QTime>

#include <QString>
#include <QLabel>
#include <QMovie>
#include <QPushButton>
#include <QGridLayout>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include "mysql.h"

namespace Ui {
    class ProgressDialog;
}
class serial;

class ProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressDialog(QWidget *parent = 0,
                            QSqlDatabase db_parent = QSqlDatabase(),
                            int user_id = 0);
    ~ProgressDialog();

    QStringList assessor;
    QStringList certification;

    void Export(QString file_path);
    void Import(QString file_path);
    bool cancel;
    int progressVal;
    QStringList get_data(QString path);
    void set_data(QStringList data);

    mysql database;
    QSqlDatabase db;
    int id_user;

    void insertAssessor(
            QString fullName,
            QString identNumber,
            QString birthPlace,
            QString birthDate,
            QString gender,
            QString homeAddress,
            QString homePhone,
            QString officeAddress,
            QString officePhone,
            QString emailAddress);
    void insertCertification(QString rfid,
            QString photoFile,
            QString certNumber,
            QString certFile,
            QString registrationNumber,
            QString kbjiNumber,
            QString competence,
            QString fullName,
            QString identNumber,
            QString birthPlace,
            QString birthDate,
            QString gender,
            QString homeAddress,
            QString homePhone,
            QString officeAddress,
            QString officePhone,
            QString emailAddress,
            QString startDate,
            QString endDate,
            QString assessor,
            QString assessmentPlace,
            QString assessmentDate);
    void save_file(QString path, QString newName);

    QStringList get_assessor();
    QStringList get_certification();
    void save_file(QString path);

private slots:
    void on_pbDetail_clicked();
    void on_pbCancel_clicked();

private:
    Ui::ProgressDialog *ui;
};

#endif // PROGRESSDIALOG_H
