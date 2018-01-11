#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, int user_id, int user_type) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Lembaga Sertifikasi Profesi Elektronika");

    db = database.connect_db("LSPE_MainWindow");

    id_user = user_id;
    role_id = user_type;
    setMainWindow();
    setTabWindow();

    ui->tabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setMainWindow()
{
    if (role_id == 1) {
        ui->pb_sertifikat_addNew->hide();
        ui->pb_sertifikat_delete->hide();
    } else if (role_id == 2) {
        ui->tabWidget->removeTab(0);
    } else if (role_id == 3) {
        ui->tabWidget->removeTab(2);
        ui->tabWidget->removeTab(0);
    } else {
        ui->tabWidget->removeTab(2);
        ui->tabWidget->removeTab(1);
        ui->tabWidget->removeTab(0);
    }
}

void MainWindow::setTabWindow()
{
    if (role_id == 1) {
        get_admin();
        get_sertifikat();
        get_assessor();
    } else if (role_id == 2) {
        get_sertifikat();
        get_assessor();
    } else if (role_id == 3) {
        get_sertifikat();
    }

    row_focus_admin = -1;
    row_focus_sertifikat = -1;
    row_focus_assessor = -1;
}

void MainWindow::get_admin()
{
    QStringList result = database.read(db,
                                  "select                                                                                   \
                                       u.id,                                                                                \
                                       u.username,                                                                          \
                                       (select name from user_type where id = u.type_id) as type,                           \
                                       i.name as full_name,                                                                 \
                                       (select number from number where id = i.ident_number_id) as identification_number,   \
                                       i.gender,                                                                            \
                                       (select address from address where id = i.home_address_id) as home_address,          \
                                       (select number from phone where id = i.home_phone_id) as home_phone_number,          \
                                       (select address from address where id = i.office_address_id) as office_address,      \
                                       (select number from phone where id = i.office_phone_id) as office_phone_number,      \
                                       (select address from address where id = i.email_address_id) as email_address         \
                                   from                                                                                     \
                                       user as u,                                                                           \
                                       identification as i                                                                  \
                                   where                                                                                    \
                                       u.identification_id = i.id"
            );

    QStandardItemModel *adminModel = new QStandardItemModel(result.length()/9, 9, this);
    adminModel->setHorizontalHeaderItem(0, new QStandardItem(QString("ID")));
    adminModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Username")));
    adminModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Type")));
    adminModel->setHorizontalHeaderItem(3, new QStandardItem(QString("Full Name")));
    adminModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Identification Number")));
    adminModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Gender")));
    adminModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Home Address")));
    adminModel->setHorizontalHeaderItem(7, new QStandardItem(QString("Home Phone Address")));
    adminModel->setHorizontalHeaderItem(8, new QStandardItem(QString("Office Address")));
    adminModel->setHorizontalHeaderItem(9, new QStandardItem(QString("Office Phone Number")));
    adminModel->setHorizontalHeaderItem(10, new QStandardItem(QString("Email Address")));

    int row = 0;
    for (int i = 0; i < result.length(); i+=11) {
        adminModel->setItem(row, 0, new QStandardItem(QString(result.at(i+0))));
        adminModel->setItem(row, 1, new QStandardItem(QString(result.at(i+1))));
        adminModel->setItem(row, 2, new QStandardItem(QString(result.at(i+2))));
        adminModel->setItem(row, 3, new QStandardItem(QString(result.at(i+3))));
        adminModel->setItem(row, 4, new QStandardItem(QString(result.at(i+4))));
        adminModel->setItem(row, 5, new QStandardItem(QString(result.at(i+5))));
        adminModel->setItem(row, 6, new QStandardItem(QString(result.at(i+6))));
        adminModel->setItem(row, 7, new QStandardItem(QString(result.at(i+7))));
        adminModel->setItem(row, 8, new QStandardItem(QString(result.at(i+8))));
        adminModel->setItem(row, 9, new QStandardItem(QString(result.at(i+9))));
        adminModel->setItem(row, 10, new QStandardItem(QString(result.at(i+10))));
        row++;
    }

    ui->tbl_admin->setModel(adminModel);
    ui->tbl_admin->setColumnWidth(0, 50);
    ui->tbl_admin->setColumnHidden(0, true);
    ui->tbl_admin->setColumnWidth(1, 100);
    ui->tbl_admin->setColumnWidth(2, 200);
    ui->tbl_admin->setColumnWidth(3, 200);
    ui->tbl_admin->setColumnWidth(4, 200);
    ui->tbl_admin->setColumnWidth(5, 200);
    ui->tbl_admin->setColumnWidth(6, 200);
    ui->tbl_admin->setColumnWidth(7, 200);
    ui->tbl_admin->setColumnWidth(8, 200);
    ui->tbl_admin->setColumnWidth(9, 200);
    ui->tbl_admin->setColumnWidth(10, 200);

    ui->cb_search_admin->addItem("Username");
    ui->cb_search_admin->addItem("Type");
    ui->cb_search_admin->addItem("Ful Name");
    ui->cb_search_admin->addItem("Identification Number");
    ui->cb_search_admin->addItem("Gender");
    ui->cb_search_admin->addItem("Home Address");
    ui->cb_search_admin->addItem("Home Phone Address");
    ui->cb_search_admin->addItem("Office Address");
    ui->cb_search_admin->addItem("Office Phone Number");
    ui->cb_search_admin->addItem("Email Address");
}

void MainWindow::get_sertifikat()
{
    QStringList result = database.read(db,
                                  "select                                                                                   \
                                       c.id,                                                                                \
                                       i.name as full_name,                                                                 \
                                       (select number from number where id = i.ident_number_id) as identification_number,   \
                                       (select number from rfid where id = c.rfid) as rfid,                                 \
                                       i.gender,                                                                            \
                                       (select number from number where id = c.cert_number_id) as certification_number,     \
                                       (select number from number where id = c.reg_number_id) as registration_number,       \
                                       (select number from number where id = c.kbji_number_id) as kbji_number,              \
                                       c.competence,                                                                        \
                                       from_unixtime(c.start_date) as start_date,                                           \
                                       from_unixtime(c.end_date) as end_date,                                               \
                                       (select address from address where id = i.home_address_id) as home_address,          \
                                       (select number from phone where id = i.home_phone_id) as home_phone_number,          \
                                       (select address from address where id = i.office_address_id) as office_address,      \
                                       (select number from phone where id = i.office_phone_id) as office_phone_number,      \
                                       (select address from address where id = i.email_address_id) as email_address,        \
                                       (select name from identification where id = (                                        \
                                            select identification_id from assessor where id = c.assessor_id                  \
                                       )) as assessor,                                                                      \
                                       c.assessment_date                                                                      \
                                   from                                                                                     \
                                       certification as c,                                                                  \
                                       identification as i                                                                  \
                                   where                                                                                    \
                                       c.identification_id = i.id"
            );

    QStandardItemModel *sertifikatModel = new QStandardItemModel(result.length()/14, 14, this);
    sertifikatModel->setHorizontalHeaderItem(0, new QStandardItem(QString("ID")));
    sertifikatModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Full Name")));
    sertifikatModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Identification Number")));
    sertifikatModel->setHorizontalHeaderItem(3, new QStandardItem(QString("RFID")));
    sertifikatModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Gender")));
    sertifikatModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Certification Number")));
    sertifikatModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Registration Number")));
    sertifikatModel->setHorizontalHeaderItem(7, new QStandardItem(QString("KBJI Number")));
    sertifikatModel->setHorizontalHeaderItem(8, new QStandardItem(QString("Competence")));
    sertifikatModel->setHorizontalHeaderItem(9, new QStandardItem(QString("Start Date")));
    sertifikatModel->setHorizontalHeaderItem(10, new QStandardItem(QString("End Date")));
    sertifikatModel->setHorizontalHeaderItem(11, new QStandardItem(QString("Home Address")));
    sertifikatModel->setHorizontalHeaderItem(12, new QStandardItem(QString("Home Phone Number")));
    sertifikatModel->setHorizontalHeaderItem(13, new QStandardItem(QString("Office Address")));
    sertifikatModel->setHorizontalHeaderItem(14, new QStandardItem(QString("Office Phone Number")));
    sertifikatModel->setHorizontalHeaderItem(15, new QStandardItem(QString("Email")));
    sertifikatModel->setHorizontalHeaderItem(16, new QStandardItem(QString("Assessor")));
    sertifikatModel->setHorizontalHeaderItem(17, new QStandardItem(QString("Assessment Date")));

    int row = 0;
    for (int i = 0; i < result.length(); i+=18) {
        sertifikatModel->setItem(row, 0, new QStandardItem(QString(result.at(i+0))));
        sertifikatModel->setItem(row, 1, new QStandardItem(QString(result.at(i+1))));
        sertifikatModel->setItem(row, 2, new QStandardItem(QString(result.at(i+2))));
        sertifikatModel->setItem(row, 3, new QStandardItem(QString(result.at(i+3))));
        sertifikatModel->setItem(row, 4, new QStandardItem(QString(result.at(i+4))));
        sertifikatModel->setItem(row, 5, new QStandardItem(QString(result.at(i+5))));
        sertifikatModel->setItem(row, 6, new QStandardItem(QString(result.at(i+6))));
        sertifikatModel->setItem(row, 7, new QStandardItem(QString(result.at(i+7))));
        sertifikatModel->setItem(row, 8, new QStandardItem(QString(result.at(i+8))));
        sertifikatModel->setItem(row, 9, new QStandardItem(QString(QDate::fromString(result.at(i+9), "dd-MM-yyyy").toString("dd MMMM yyyy"))));
        sertifikatModel->setItem(row, 10, new QStandardItem(QString(QDate::fromString(result.at(i+10), "dd-MM-yyyy").toString("dd MMMM yyyy"))));
        sertifikatModel->setItem(row, 11, new QStandardItem(QString(result.at(i+11))));
        sertifikatModel->setItem(row, 12, new QStandardItem(QString(result.at(i+12))));
        sertifikatModel->setItem(row, 13, new QStandardItem(QString(result.at(i+13))));
        sertifikatModel->setItem(row, 14, new QStandardItem(QString(result.at(i+14))));
        sertifikatModel->setItem(row, 15, new QStandardItem(QString(result.at(i+15))));
        sertifikatModel->setItem(row, 16, new QStandardItem(QString(result.at(i+16))));
        sertifikatModel->setItem(row, 17, new QStandardItem(QString(QDate::fromString(result.at(i+17), "dd-MM-yyyy").toString("dd MMMM yyyy"))));
        row++;
    }

    ui->tbl_sertifikat->setModel(sertifikatModel);
    ui->tbl_sertifikat->setColumnWidth(0, 50);
    ui->tbl_sertifikat->setColumnHidden(0, true);
    ui->tbl_sertifikat->setColumnWidth(1, 200);
    ui->tbl_sertifikat->setColumnWidth(2, 200);
    ui->tbl_sertifikat->setColumnWidth(3, 200);
    ui->tbl_sertifikat->setColumnWidth(4, 200);
    ui->tbl_sertifikat->setColumnWidth(5, 200);
    ui->tbl_sertifikat->setColumnWidth(6, 200);
    ui->tbl_sertifikat->setColumnWidth(7, 100);
    ui->tbl_sertifikat->setColumnWidth(8, 100);
    ui->tbl_sertifikat->setColumnWidth(9, 200);
    ui->tbl_sertifikat->setColumnWidth(10, 200);
    ui->tbl_sertifikat->setColumnWidth(11, 200);
    ui->tbl_sertifikat->setColumnWidth(12, 200);
    ui->tbl_sertifikat->setColumnWidth(13, 200);
    ui->tbl_sertifikat->setColumnWidth(14, 200);
    ui->tbl_sertifikat->setColumnWidth(15, 200);
    ui->tbl_sertifikat->setColumnWidth(16, 200);
    ui->tbl_sertifikat->setColumnWidth(17, 200);

    ui->cb_search_sertifikat->addItem("Full Name");
    ui->cb_search_sertifikat->addItem("Identification Number");
    ui->cb_search_sertifikat->addItem("RFID");
    ui->cb_search_sertifikat->addItem("Gender");
    ui->cb_search_sertifikat->addItem("Certification Number");
    ui->cb_search_sertifikat->addItem("Registration Number");
    ui->cb_search_sertifikat->addItem("KBJI Number");
    ui->cb_search_sertifikat->addItem("Competence");
    ui->cb_search_sertifikat->addItem("Start Date");
    ui->cb_search_sertifikat->addItem("End Date");
    ui->cb_search_sertifikat->addItem("Home Address");
    ui->cb_search_sertifikat->addItem("Home Phone Number");
    ui->cb_search_sertifikat->addItem("Office Address");
    ui->cb_search_sertifikat->addItem("Office Phone Number");
    ui->cb_search_sertifikat->addItem("Email");
    ui->cb_search_sertifikat->addItem("Assessor");
    ui->cb_search_sertifikat->addItem("Assessment Date");
}

void MainWindow::get_assessor()
{
    QStringList result = database.read(db,
                                  "select                                                                                   \
                                       a.id,                                                                                \
                                       i.name as full_name,                                                                 \
                                       (select number from number where id = i.ident_number_id) as identification_number,   \
                                       i.gender,                                                                            \
                                       (select address from address where id = i.home_address_id) as home_address,          \
                                       (select number from phone where id = i.home_phone_id) as home_phone_number,          \
                                       (select address from address where id = i.office_address_id) as office_address,      \
                                       (select number from phone where id = i.office_phone_id) as office_phone_number,      \
                                       (select address from address where id = i.email_address_id) as email_address         \
                                   from                                                                                     \
                                       assessor as a,                                                                       \
                                       identification as i                                                                  \
                                   where                                                                                    \
                                       a.identification_id = i.id and a.status = 1"
            );

    QStandardItemModel *assessorModel = new QStandardItemModel(result.length()/7, 7, this);
    assessorModel->setHorizontalHeaderItem(0, new QStandardItem(QString("ID")));
    assessorModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Full Name")));
    assessorModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Identification Number")));
    assessorModel->setHorizontalHeaderItem(3, new QStandardItem(QString("Gender")));
    assessorModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Home Address")));
    assessorModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Home Phone Address")));
    assessorModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Office Address")));
    assessorModel->setHorizontalHeaderItem(7, new QStandardItem(QString("Office Phone Number")));
    assessorModel->setHorizontalHeaderItem(8, new QStandardItem(QString("Email")));

    int row = 0;
    for (int i = 0; i < result.length(); i+=9) {
        assessorModel->setItem(row, 0, new QStandardItem(QString(result.at(i+0))));
        assessorModel->setItem(row, 1, new QStandardItem(QString(result.at(i+1))));
        assessorModel->setItem(row, 2, new QStandardItem(QString(result.at(i+2))));
        assessorModel->setItem(row, 3, new QStandardItem(QString(result.at(i+3))));
        assessorModel->setItem(row, 4, new QStandardItem(QString(result.at(i+4))));
        assessorModel->setItem(row, 5, new QStandardItem(QString(result.at(i+5))));
        assessorModel->setItem(row, 6, new QStandardItem(QString(result.at(i+6))));
        assessorModel->setItem(row, 7, new QStandardItem(QString(result.at(i+7))));
        assessorModel->setItem(row, 8, new QStandardItem(QString(result.at(i+8))));
        row++;
    }

    ui->tbl_assessor->setModel(assessorModel);
    ui->tbl_assessor->setColumnWidth(0, 50);
    ui->tbl_assessor->setColumnHidden(0, true);
    ui->tbl_assessor->setColumnWidth(1, 200);
    ui->tbl_assessor->setColumnWidth(2, 200);
    ui->tbl_assessor->setColumnWidth(3, 200);
    ui->tbl_assessor->setColumnWidth(4, 200);
    ui->tbl_assessor->setColumnWidth(5, 200);
    ui->tbl_assessor->setColumnWidth(6, 200);
    ui->tbl_assessor->setColumnWidth(7, 200);
    ui->tbl_assessor->setColumnWidth(8, 200);

    ui->cb_search_assessor->addItem("Ful Name");
    ui->cb_search_assessor->addItem("Identification Number");
    ui->cb_search_assessor->addItem("Gender");
    ui->cb_search_assessor->addItem("Home Address");
    ui->cb_search_assessor->addItem("Home Phone Address");
    ui->cb_search_assessor->addItem("Office Address");
    ui->cb_search_assessor->addItem("Office Phone Number");
    ui->cb_search_assessor->addItem("Email");
}

void MainWindow::on_pb_admin_addNew_clicked()
{
    Form_Pendaftaran *daftar_admin_baru = new Form_Pendaftaran(this, db, 1, id_user, false, 0);
    daftar_admin_baru->exec();

    daftar_admin_baru->deleteLater();
    setTabWindow();
}

void MainWindow::on_pb_admin_delete_clicked()
{
    if (row_focus_admin > -1) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete Administrator", "Are you sure want to delete ??",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            delete_admin(ui->tbl_admin->model()->data(ui->tbl_admin->model()->index(row_focus_admin,0)).toInt());
        }
    } else {
        QMessageBox::information(this, "Delete Administrator", "Please, Select some row first ..");
    }
}

void MainWindow::on_pb_assessor_addNew_clicked()
{
    Form_Pendaftaran *daftar_assessor_baru = new Form_Pendaftaran(this, db, 2, id_user, false, 0);
    daftar_assessor_baru->exec();

//    qDebug() << "Selesai Daftar assessor Baru";
    daftar_assessor_baru->deleteLater();
    setTabWindow();
}

void MainWindow::on_pb_assessor_delete_clicked()
{
    if (row_focus_assessor > -1) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete assessor", "Are you sure want to delete ??",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            delete_assessor(ui->tbl_assessor->model()->data(ui->tbl_assessor->model()->index(row_focus_assessor,0)).toInt());
        }
    } else {
        QMessageBox::information(this, "Delete assessor", "Please, Select some row first ..");
    }
}

void MainWindow::on_tbl_admin_doubleClicked(const QModelIndex &index)
{
    int id = ui->tbl_admin->model()->data(ui->tbl_admin->model()->index(index.row(),0)).toInt();
    Form_Pendaftaran *edit_admin = new Form_Pendaftaran(this, db, 1, id_user, true, id);
    edit_admin->exec();

    edit_admin->deleteLater();
    setTabWindow();
}

void MainWindow::on_tbl_assessor_doubleClicked(const QModelIndex &index)
{
    int id = ui->tbl_assessor->model()->data(ui->tbl_assessor->model()->index(index.row(),0)).toInt();
    Form_Pendaftaran *edit_assessor = new Form_Pendaftaran(this, db, 2, id_user, true, id);
    edit_assessor->exec();

    edit_assessor->deleteLater();
    setTabWindow();
}

void MainWindow::on_tbl_sertifikat_doubleClicked(const QModelIndex &index)
{
    int id = ui->tbl_sertifikat->model()->data(ui->tbl_sertifikat->model()->index(index.row(),0)).toInt();
    Form_Peserta *view_peserta = new Form_Peserta(this, db, id, id_user);
    view_peserta->showMaximized();
    view_peserta->exec();

    view_peserta->deleteLater();
    setTabWindow();
}

void MainWindow::on_pb_sertifikat_addNew_clicked()
{
    Form_Pendaftaran *daftar_sertifikat_baru = new Form_Pendaftaran(this, db, 3, id_user, false, 0);
    daftar_sertifikat_baru->exec();

//    qDebug() << "Selesai Daftar assessor Baru";
    daftar_sertifikat_baru->deleteLater();
    setTabWindow();
}

void MainWindow::on_pb_sertifikat_delete_clicked()
{
    if (row_focus_sertifikat > -1) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete Sertifikat", "Are you sure want to delete ??",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            delete_sertifikat(ui->tbl_sertifikat->model()->data(ui->tbl_sertifikat->model()->index(row_focus_sertifikat,0)).toInt());
        }
    } else {
        QMessageBox::information(this, "Delete Sertifikat", "Please, Select some row first ..");
    }
}

void MainWindow::on_pb_search_admin_clicked()
{
    row_focus_admin = search(ui->cb_search_admin->currentIndex()+1, ui->le_search_admin, ui->tbl_admin);
}

void MainWindow::on_pb_search_sertifikat_clicked()
{
    row_focus_sertifikat = search(ui->cb_search_sertifikat->currentIndex()+1, ui->le_search_sertifikat, ui->tbl_sertifikat);
}

void MainWindow::on_pb_search_assessor_clicked()
{
    row_focus_assessor = search(ui->cb_search_assessor->currentIndex()+1, ui->le_search_assessor, ui->tbl_assessor);
}

int MainWindow::search(int column, QLineEdit *le_search, QTableView *table_target)
{
    QString findText = le_search->text().trimmed();
    QAbstractItemModel *model = table_target->model();
    QSortFilterProxyModel proxy;
    proxy.setSourceModel(model);
    proxy.setFilterKeyColumn(column);
    proxy.setFilterFixedString(findText);
    QModelIndex matchingIndex = proxy.mapToSource(proxy.index(0,0));
    QItemSelectionModel::SelectionFlags flags = QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows;
    if(matchingIndex.isValid()) {
//        table_target->scrollTo(matchingIndex);
        table_target->selectionModel()->select(matchingIndex, flags);
        return matchingIndex.row();
    } else {
        QMessageBox::information(this,"Search Engine", findText + " not found!");
        return -1;
    }
}

void MainWindow::on_le_search_admin_returnPressed()
{
    ui->pb_search_admin->click();
}

void MainWindow::on_le_search_sertifikat_returnPressed()
{
    ui->pb_search_sertifikat->click();
}

void MainWindow::on_le_search_assessor_returnPressed()
{
    ui->pb_search_assessor->click();
}

void MainWindow::on_tbl_admin_clicked(const QModelIndex &index)
{
    row_focus_admin = index.row();
}

void MainWindow::on_tbl_sertifikat_clicked(const QModelIndex &index)
{
    row_focus_sertifikat = index.row();
}

void MainWindow::on_tbl_assessor_clicked(const QModelIndex &index)
{
    row_focus_assessor = index.row();
}

void MainWindow::delete_admin(int id)
{
    database.write(db, "delete from number where id = (                             \
                            select ident_number_id                                  \
                            from identification where id = (                        \
                                select identification_id                            \
                                from user where id = " + QString::number(id) + "    \
                            )                                                       \
                        )"
            );
    database.write(db, "delete from address where id = (                            \
                            select home_address_id                                  \
                            from identification where id = (                        \
                                select identification_id                            \
                                from user where id = " + QString::number(id) + "    \
                            )                                                       \
                        )"
            );
    database.write(db, "delete from phone where id = (                              \
                            select home_phone_id                                    \
                            from identification where id = (                        \
                                select identification_id                            \
                                from user where id = " + QString::number(id) + "    \
                            )                                                       \
                        )"
            );
    database.write(db, "delete from address where id = (                            \
                            select office_address_id                                \
                            from identification where id = (                        \
                                select identification_id                            \
                                from user where id = " + QString::number(id) + "    \
                            )                                                       \
                        )"
            );
    database.write(db, "delete from phone where id = (                              \
                            select office_phone_id                                  \
                            from identification where id = (                        \
                                select identification_id                            \
                                from user where id = " + QString::number(id) + "    \
                            )                                                       \
                        )"
            );
    database.write(db, "delete from address where id = (                            \
                            select email_address_id                                 \
                            from identification where id = (                        \
                                select identification_id                            \
                                from user where id = " + QString::number(id) + "    \
                            )                                                       \
                        )"
            );
    database.write(db, "delete from identification where id = (                 \
                            select identification_id                            \
                            from user where id = " + QString::number(id) + "    \
                        )"
            );
    database.write(db, "delete from user where id = " + QString::number(id));
    setTabWindow();
}

void MainWindow::delete_assessor(int id)
{
    database.write(db, "update assessor set status = 0 where id = " + QString::number(id));
    setTabWindow();
}

void MainWindow::delete_sertifikat(int id)
{
    database.write(db, "delete from number where id = (                 \
                            select ident_number_id                      \
                            from identification where id = (            \
                                select identification_id                \
                                from certification                      \
                                where id = " + QString::number(id) + "  \
                            )                                           \
                        )"
            );
    database.write(db, "delete from address where id = (                \
                            select home_address_id                      \
                            from identification where id = (            \
                                select identification_id                \
                                from certification                      \
                                where id = " + QString::number(id) + "  \
                            )                                           \
                        )"
            );
    database.write(db, "delete from phone where id = (                  \
                            select home_phone_id                        \
                            from identification where id = (            \
                                select identification_id                \
                                from certification                      \
                                where id = " + QString::number(id) + "  \
                            )                                           \
                        )"
            );
    database.write(db, "delete from address where id = (                \
                            select office_address_id                    \
                            from identification where id = (            \
                                select identification_id                \
                                from certification                      \
                                where id = " + QString::number(id) + "  \
                            )                                           \
                        )"
            );
    database.write(db, "delete from phone where id = (                  \
                            select office_phone_id                      \
                            from identification where id = (            \
                                select identification_id                \
                                from certification                      \
                                where id = " + QString::number(id) + "  \
                            )                                           \
                        )"
            );
    database.write(db, "delete from address where id = (                \
                            select email_address_id                     \
                            from identification where id = (            \
                                select identification_id                \
                                from certification                      \
                                where id = " + QString::number(id) + "  \
                            )                                           \
                        )"
            );
    database.write(db, "delete from identification where id = (     \
                            select identification_id                \
                            from certification                      \
                            where id = " + QString::number(id) + "  \
                        )"
            );
    database.write(db, "delete from picture where id = (            \
                            select photo_picture_id                 \
                            from certification                      \
                            where id = " + QString::number(id) + "  \
                        )"
            );
    database.write(db, "delete from number where id = (             \
                            select cert_number_id                   \
                            from certification                      \
                            where id = " + QString::number(id) + "  \
                        )"
            );
    database.write(db, "delete from picture where id = (            \
                            select certification_picture_id         \
                            from certification                      \
                            where id = " + QString::number(id) + "  \
                        )"
            );
    database.write(db, "delete from number where id = (             \
                            select reg_number_id                    \
                            from certification                      \
                            where id = " + QString::number(id) + "  \
                        )"
            );
    database.write(db, "delete from number where id = (             \
                            select kbji_number_id                   \
                            from certification                      \
                            where id = " + QString::number(id) + "  \
                        )"
            );
    database.write(db, "delete from address where id = (            \
                            select assessment_place_id              \
                            from certification                      \
                            where id = " + QString::number(id) + "  \
                        )"
            );
    database.write(db, "delete from certification where id = " + QString::number(id));
    setTabWindow();
}
