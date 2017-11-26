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
        get_assesor();
    } else if (role_id == 2) {
        get_sertifikat();
        get_assesor();
    } else if (role_id == 3) {
        get_sertifikat();
    }
}

void MainWindow::get_admin()
{
    QStringList result = database.read(db,
                                  "select                                                                                   \
                                       u.username,                                                                          \
                                       (select name from user_type where id = u.type_id) as type,                           \
                                       i.name,                                                                              \
                                       (select number from number where id = i.ident_number_id) as identification_number,   \
                                       (select address from address where id = i.home_address_id) as home_address,          \
                                       (select number from phone where id = i.home_phone_id) as home_phone_number,          \
                                       (select address from address where id = i.office_address_id) as office_address,      \
                                       (select number from phone where id = i.office_phone_id) as office_phone_number       \
                                   from                                                                                     \
                                       user as u,                                                                           \
                                       identification as i                                                                  \
                                   where                                                                                    \
                                       u.identification_id = i.id"
            );

    QStandardItemModel *adminModel = new QStandardItemModel(result.length()/8, 8, this);
    adminModel->setHorizontalHeaderItem(0, new QStandardItem(QString("Username")));
    adminModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Type")));
    adminModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Full Name")));
    adminModel->setHorizontalHeaderItem(3, new QStandardItem(QString("Identification Number")));
    adminModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Home Address")));
    adminModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Home Phone Address")));
    adminModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Office Address")));
    adminModel->setHorizontalHeaderItem(7, new QStandardItem(QString("Office Phone Number")));

    int row = 0;
    for (int i = 0; i < result.length(); i+=8) {
        adminModel->setItem(row, 0, new QStandardItem(QString(result.at(i+0))));
        adminModel->setItem(row, 1, new QStandardItem(QString(result.at(i+1))));
        adminModel->setItem(row, 2, new QStandardItem(QString(result.at(i+2))));
        adminModel->setItem(row, 3, new QStandardItem(QString(result.at(i+3))));
        adminModel->setItem(row, 4, new QStandardItem(QString(result.at(i+4))));
        adminModel->setItem(row, 5, new QStandardItem(QString(result.at(i+5))));
        adminModel->setItem(row, 6, new QStandardItem(QString(result.at(i+6))));
        adminModel->setItem(row, 7, new QStandardItem(QString(result.at(i+7))));
        row++;
    }

    ui->tbl_admin->setModel(adminModel);
    ui->tbl_admin->setColumnWidth(0, 100);
    ui->tbl_admin->setColumnWidth(1, 100);
    ui->tbl_admin->setColumnWidth(2, 200);
    ui->tbl_admin->setColumnWidth(3, 200);
    ui->tbl_admin->setColumnWidth(4, 200);
    ui->tbl_admin->setColumnWidth(5, 200);
    ui->tbl_admin->setColumnWidth(6, 200);
    ui->tbl_admin->setColumnWidth(7, 200);
}

void MainWindow::get_sertifikat()
{
    QStringList result = database.read(db,
                                  "select                                                                                   \
                                       i.name as full_name,                                                                 \
                                       (select number from number where id = i.ident_number_id) as identification_number,   \
                                       c.rfid as rfid,                                                                      \
                                       (select number from number where id = c.cert_number_id) as certification_number,     \
                                       (select number from number where id = c.reg_number_id) as registration_number,       \
                                       (select number from number where id = c.kbji_number_id) as kbji_number,              \
                                       (select name from competence where id = c.competence_id) as competence,              \
                                       from_unixtime(c.start_date) as start_date,                                           \
                                       from_unixtime(c.end_date) as end_date,                                               \
                                       (select address from address where id = i.home_address_id) as home_address,          \
                                       (select number from phone where id = i.home_phone_id) as home_phone_number,          \
                                       (select address from address where id = i.office_address_id) as office_address,      \
                                       (select number from phone where id = i.office_phone_id) as office_phone_number       \
                                   from                                                                                     \
                                       certification as c,                                                                  \
                                       identification as i                                                                  \
                                   where                                                                                    \
                                       c.identification_id = i.id"
            );

    QStandardItemModel *sertifikatModel = new QStandardItemModel(result.length()/13, 13, this);
    sertifikatModel->setHorizontalHeaderItem(0, new QStandardItem(QString("Full Name")));
    sertifikatModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Identification Number")));
    sertifikatModel->setHorizontalHeaderItem(2, new QStandardItem(QString("RFID")));
    sertifikatModel->setHorizontalHeaderItem(3, new QStandardItem(QString("Certification Number")));
    sertifikatModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Registration Number")));
    sertifikatModel->setHorizontalHeaderItem(5, new QStandardItem(QString("KBJI Number")));
    sertifikatModel->setHorizontalHeaderItem(6, new QStandardItem(QString("Competence")));
    sertifikatModel->setHorizontalHeaderItem(7, new QStandardItem(QString("Start Date")));
    sertifikatModel->setHorizontalHeaderItem(8, new QStandardItem(QString("End Date")));
    sertifikatModel->setHorizontalHeaderItem(9, new QStandardItem(QString("Home Address")));
    sertifikatModel->setHorizontalHeaderItem(10, new QStandardItem(QString("Home Phone Number")));
    sertifikatModel->setHorizontalHeaderItem(11, new QStandardItem(QString("Office Address")));
    sertifikatModel->setHorizontalHeaderItem(12, new QStandardItem(QString("Office Phone Number")));

    int row = 0;
    for (int i = 0; i < result.length(); i+=13) {
        sertifikatModel->setItem(row, 0, new QStandardItem(QString(result.at(i+0))));
        sertifikatModel->setItem(row, 1, new QStandardItem(QString(result.at(i+1))));
        sertifikatModel->setItem(row, 2, new QStandardItem(QString(result.at(i+2))));
        sertifikatModel->setItem(row, 3, new QStandardItem(QString(result.at(i+3))));
        sertifikatModel->setItem(row, 4, new QStandardItem(QString(result.at(i+4))));
        sertifikatModel->setItem(row, 5, new QStandardItem(QString(result.at(i+5))));
        sertifikatModel->setItem(row, 6, new QStandardItem(QString(result.at(i+6))));
        sertifikatModel->setItem(row, 7, new QStandardItem(QString(result.at(i+7))));
        sertifikatModel->setItem(row, 8, new QStandardItem(QString(result.at(i+8))));
        sertifikatModel->setItem(row, 9, new QStandardItem(QString(result.at(i+9))));
        sertifikatModel->setItem(row, 10, new QStandardItem(QString(result.at(i+10))));
        sertifikatModel->setItem(row, 11, new QStandardItem(QString(result.at(i+11))));
        sertifikatModel->setItem(row, 12, new QStandardItem(QString(result.at(i+12))));
        row++;
    }

    ui->tbl_sertifikat->setModel(sertifikatModel);
    ui->tbl_sertifikat->setColumnWidth(0, 200);
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
}

void MainWindow::get_assesor()
{
    QStringList result = database.read(db,
                                  "select                                                                                   \
                                       i.name as full_name,                                                                 \
                                       (select number from number where id = i.ident_number_id) as identification_number,   \
                                       (select address from address where id = i.home_address_id) as home_address,          \
                                       (select number from phone where id = i.home_phone_id) as home_phone_number,          \
                                       (select address from address where id = i.office_address_id) as office_address,      \
                                       (select number from phone where id = i.office_phone_id) as office_phone_number       \
                                   from                                                                                     \
                                       user as u,                                                                           \
                                       identification as i                                                                  \
                                   where                                                                                    \
                                       u.identification_id = i.id and                                                       \
                                       u.type_id = 3"
            );

    QStandardItemModel *assesorModel = new QStandardItemModel(result.length()/6, 6, this);
    assesorModel->setHorizontalHeaderItem(0, new QStandardItem(QString("Full Name")));
    assesorModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Identification Number")));
    assesorModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Home Address")));
    assesorModel->setHorizontalHeaderItem(3, new QStandardItem(QString("Home Phone Address")));
    assesorModel->setHorizontalHeaderItem(4, new QStandardItem(QString("Office Address")));
    assesorModel->setHorizontalHeaderItem(5, new QStandardItem(QString("Office Phone Number")));

    int row = 0;
    for (int i = 0; i < result.length(); i+=6) {
        assesorModel->setItem(row, 0, new QStandardItem(QString(result.at(i+0))));
        assesorModel->setItem(row, 1, new QStandardItem(QString(result.at(i+1))));
        assesorModel->setItem(row, 2, new QStandardItem(QString(result.at(i+2))));
        assesorModel->setItem(row, 3, new QStandardItem(QString(result.at(i+3))));
        assesorModel->setItem(row, 4, new QStandardItem(QString(result.at(i+4))));
        assesorModel->setItem(row, 5, new QStandardItem(QString(result.at(i+5))));
        row++;
    }

    ui->tbl_assesor->setModel(assesorModel);
    ui->tbl_assesor->setColumnWidth(0, 200);
    ui->tbl_assesor->setColumnWidth(1, 200);
    ui->tbl_assesor->setColumnWidth(2, 200);
    ui->tbl_assesor->setColumnWidth(3, 200);
    ui->tbl_assesor->setColumnWidth(4, 200);
    ui->tbl_assesor->setColumnWidth(5, 200);
}

void MainWindow::on_pb_admin_addNew_clicked()
{
    Form_Pendaftaran *daftar_admin_baru = new Form_Pendaftaran(this, db, 1, id_user, false, 0);
    daftar_admin_baru->exec();

//    qDebug() << "Selesai Daftar Admin Baru";
    daftar_admin_baru->deleteLater();
    setTabWindow();
}

void MainWindow::on_pb_admin_delete_clicked()
{

}

void MainWindow::on_pb_assesor_addNew_clicked()
{
    Form_Pendaftaran *daftar_assesor_baru = new Form_Pendaftaran(this, db, 2, id_user, false, 0);
    daftar_assesor_baru->exec();

//    qDebug() << "Selesai Daftar Assesor Baru";
    daftar_assesor_baru->deleteLater();
    setTabWindow();
}

void MainWindow::on_pb_assesor_delete_clicked()
{

}
