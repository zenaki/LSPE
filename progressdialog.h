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

namespace Ui {
    class ProgressDialog;
}
class serial;

class ProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressDialog(QWidget *parent = 0);
    ~ProgressDialog();

    void Processing(QString file_path);
    bool cancel;
    int progressVal;
    QStringList get_data(QString path);
    void set_data(QStringList data);

private slots:
    void on_pbDetail_clicked();
    void on_pbCancel_clicked();

private:
    Ui::ProgressDialog *ui;
};

#endif // PROGRESSDIALOG_H
