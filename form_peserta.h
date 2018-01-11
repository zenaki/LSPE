#ifndef FORM_PESERTA_H
#define FORM_PESERTA_H

#include <QDialog>
#include "qpdflib/QPdfWidget"

#include "mysql.h"
#include "form_pendaftaran.h"

namespace Ui {
class Form_Peserta;
}

class Form_Peserta : public QDialog
{
    Q_OBJECT

public:
    explicit Form_Peserta(QWidget *parent = 0,
                          QSqlDatabase db_parent = QSqlDatabase(),
                          int id = 0,
                          int id_user = 0
            );
    ~Form_Peserta();

private slots:

    void on_pb_edit_clicked();

    void on_pb_delete_clicked();

private:
    Ui::Form_Peserta *ui;

    mysql database;
    QSqlDatabase db;

    int target_id;
    int user_id;

    QAction *m_pOpenFileAction;
    QPdfWidget *m_pPdfWidget;

    void setWindow();
};

#endif // FORM_PESERTA_H
