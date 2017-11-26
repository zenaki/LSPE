#include "form_pendaftaran.h"
#include "ui_form_pendaftaran.h"

Form_Pendaftaran::Form_Pendaftaran(QWidget *parent,
                                   QSqlDatabase db_parent,
                                   int mode,
                                   int user_id,
                                   bool edit,
                                   int id
                                   ) :
    QDialog(parent),
    ui(new Ui::Form_Pendaftaran)
{
    ui->setupUi(this);
    this->setWindowTitle("Formulir");

    db = db_parent;
    id_user = user_id;
    id_target = id;
    edit_mode = edit;
    setWindow(mode);
    setEdit(edit, id_target, mode);

    ui->tabWidget->setCurrentIndex(0);
}

Form_Pendaftaran::~Form_Pendaftaran()
{
    delete ui;
}

void Form_Pendaftaran::setWindow(int mode)
{
    if (mode == 0) {
        ui->tabWidget->removeTab(2);
        ui->tabWidget->removeTab(1);
        ui->tabWidget->removeTab(0);
    } else if (mode == 1) {
        ui->tabWidget->removeTab(2);
        ui->tabWidget->removeTab(1);
    } else if (mode == 2) {
        ui->tabWidget->removeTab(2);
        ui->tabWidget->removeTab(0);
    } else if (mode == 3) {
        ui->tabWidget->removeTab(1);
        ui->tabWidget->removeTab(0);
    }
}

void Form_Pendaftaran::setEdit(bool edit, int id, int mode)
{
    if (edit) {
        if (mode == 1) {
            QStringList result = database.read(db,
                                          "select                                                                                   \
                                               u.username,                                                                          \
                                               u.password,                                                                          \
                                               i.name,                                                                              \
                                               i.birth_place,                                                                       \
                                               i.birth_date,                                                                        \
                                               (select number from number where id = i.ident_number_id) as identification_number,   \
                                               (select address from address where id = i.home_address_id) as home_address,          \
                                               (select number from phone where id = i.home_phone_id) as home_phone_number,          \
                                               (select address from address where id = i.office_address_id) as office_address,      \
                                               (select number from phone where id = i.office_phone_id) as office_phone_number       \
                                           from                                                                                     \
                                               user as u,                                                                           \
                                               identification as i                                                                  \
                                           where                                                                                    \
                                               u.identification_id = i.id                                                           \
                                               u.id = " + QString::number(id)
                    );

            ui->le_admin_username->setText(result.at(0));
            ui->le_admin_username->setEnabled(false);
            ui->le_admin_password->setText(result.at(1));
            ui->le_admin_fullName->setText(result.at(2));
            ui->le_admin_birthPlace->setText(result.at(3));
            ui->de_admin_birthDate->setDate(QDate::fromString(result.at(4), "dd-MM-yyyy"));
            ui->le_admin_identNumber->setText(result.at(5));
            ui->te_admin_homeAddress->setText(result.at(6));
            ui->le_admin_homePhone->setText(result.at(7));
            ui->te_admin_officeAddress->setText(result.at(8));
            ui->le_admin_officePhone->setText(result.at(9));
        } else if (mode == 2) {
            QStringList result = database.read(db,
                                          "select                                                                                   \
                                               u.username,                                                                          \
                                               u.password,                                                                          \
                                               i.name,                                                                              \
                                               i.birth_place,                                                                       \
                                               i.birth_date,                                                                        \
                                               (select number from number where id = i.ident_number_id) as identification_number,   \
                                               (select address from address where id = i.home_address_id) as home_address,          \
                                               (select number from phone where id = i.home_phone_id) as home_phone_number,          \
                                               (select address from address where id = i.office_address_id) as office_address,      \
                                               (select number from phone where id = i.office_phone_id) as office_phone_number       \
                                           from                                                                                     \
                                               user as u,                                                                           \
                                               identification as i                                                                  \
                                           where                                                                                    \
                                               u.identification_id = i.id                                                           \
                                               u.id = " + QString::number(id)
                    );

            ui->le_assesor_username->setText(result.at(0));
            ui->le_assesor_username->setEnabled(false);
            ui->le_assesor_password->setText(result.at(1));
            ui->le_assesor_fullName->setText(result.at(2));
            ui->le_assesor_birthPlace->setText(result.at(3));
            ui->de_assesor_birthDate->setDate(QDate::fromString(result.at(4), "dd-MM-yyyy"));
            ui->le_assesor_identNumber->setText(result.at(5));
            ui->te_assesor_homeAddress->setText(result.at(6));
            ui->le_assesor_homePhone->setText(result.at(7));
            ui->te_assesor_officeAddress->setText(result.at(8));
            ui->le_assesor_officePhone->setText(result.at(9));
        } else if (mode == 3) {

        }
    }
}

void Form_Pendaftaran::on_pb_admin_ok_clicked()
{
    QString update_date = QString::number(QDateTime::currentDateTime().toTime_t());
    if (edit_mode) {
        database.write(db, "update user                                                                 \
                            set                                                                         \
                                password = '" + ui->le_admin_password->text() + "',  \
                                update_by = '" + QString::number(id_user) + "',                         \
                                update_date = '" + update_date + "'                                     \
                            where id = '"+ QString::number(id_target) +"';"
                );
        database.write(db, "update identification                                                               \
                            set                                                                                 \
                                name = '" + ui->le_admin_fullName->text() + "',                                 \
                                birth_place = '" + ui->le_admin_birthPlace->text() + "',                        \
                                birth_date = '" + ui->de_admin_birthDate->date().toString("dd-MM-yyyy") + "',   \
                                update_by = '" + QString::number(id_user) + "',                                 \
                                update_date = '" + update_date + "'                                             \
                            where id = (select                                                                  \
                                            identification_id                                                   \
                                        from                                                                    \
                                            user                                                                \
                                        where                                                                   \
                                            id = '"+ QString::number(id_target) +"');"
                );
        database.write(db, "update number                                                       \
                            set                                                                 \
                                number = '" + ui->le_admin_identNumber->text() + "',            \
                                update_by = '" + QString::number(id_user) + "',                 \
                                update_date = '" + update_date + "'                             \
                            where id = (select                                                  \
                                            ident_number_id                                     \
                                        from                                                    \
                                            identification                                      \
                                        where                                                   \
                                            id = (select                                        \
                                                    identification_id                           \
                                                  from                                          \
                                                    user                                        \
                                                  where                                         \
                                                    id = '"+ QString::number(id_target) +"')    \
                                       );"
                );
        database.write(db, "update address                                                      \
                            set                                                                 \
                                address = '" + ui->te_admin_homeAddress->toPlainText() + "',    \
                                update_by = '" + QString::number(id_user) + "',                 \
                                update_date = '" + update_date + "'                             \
                            where id = (select                                                  \
                                            home_address_id                                     \
                                        from                                                    \
                                            identification                                      \
                                        where                                                   \
                                            id = (select                                        \
                                                    identification_id                           \
                                                  from                                          \
                                                    user                                        \
                                                  where                                         \
                                                    id = '"+ QString::number(id_target) +"')    \
                                       );"
                );
        database.write(db, "update phone                                                        \
                            set                                                                 \
                                number = '" + ui->le_admin_homePhone->text() + "',              \
                                update_by = '" + QString::number(id_user) + "',                 \
                                update_date = '" + update_date + "'                             \
                            where id = (select                                                  \
                                            home_phone_id                                       \
                                        from                                                    \
                                            identification                                      \
                                        where                                                   \
                                            id = (select                                        \
                                                    identification_id                           \
                                                  from                                          \
                                                    user                                        \
                                                  where                                         \
                                                    id = '"+ QString::number(id_target) +"')    \
                                       );"
                );
        database.write(db, "update address                                                      \
                            set                                                                 \
                                address = '" + ui->te_admin_homeAddress->toPlainText() + "',    \
                                update_by = '" + QString::number(id_user) + "',                 \
                                update_date = '" + update_date + "'                             \
                            where id = (select                                                  \
                                            office_address_id                                   \
                                        from                                                    \
                                            identification                                      \
                                        where                                                   \
                                            id = (select                                        \
                                                    identification_id                           \
                                                  from                                          \
                                                    user                                        \
                                                  where                                         \
                                                    id = '"+ QString::number(id_target) +"')    \
                                       );"
                );
        database.write(db, "update phone                                                        \
                            set                                                                 \
                                number = '" + ui->le_admin_officePhone->text() + "',            \
                                update_by = '" + QString::number(id_user) + "',                 \
                                update_date = '" + update_date + "'                             \
                            where id = (select                                                  \
                                            office_phone_id                                     \
                                        from                                                    \
                                            identification                                      \
                                        where                                                   \
                                            id = (select                                        \
                                                    identification_id                           \
                                                  from                                          \
                                                    user                                        \
                                                  where                                         \
                                                    id = '"+ QString::number(id_target) +"')    \
                                       );"
                );
    } else {
        QString new_home_address_id = database.read(db, "SELECT AUTO_INCREMENT              \
                                                         FROM  INFORMATION_SCHEMA.TABLES    \
                                                         WHERE TABLE_NAME   = 'address'").at(0);
        database.write(db, "insert into address (                                   \
                                address,                                            \
                                type_id,                                            \
                                create_by,                                          \
                                create_date,                                        \
                                update_by,                                          \
                                update_date                                         \
                            ) values (                                              \
                                '" + ui->te_admin_homeAddress->toPlainText() + "',  \
                                '1',                                                \
                                '" + QString::number(id_user) + "',                 \
                                '" + update_date + "',                              \
                                '" + QString::number(id_user) + "',                 \
                                '" + update_date + "'                               \
                            );"
        );
        QString new_home_phone_id = database.read(db, "SELECT AUTO_INCREMENT            \
                                                       FROM  INFORMATION_SCHEMA.TABLES  \
                                                       WHERE TABLE_NAME   = 'phone'").at(0);
        database.write(db, "insert into phone (                             \
                                number,                                     \
                                type_id,                                    \
                                create_by,                                  \
                                create_date,                                \
                                update_by,                                  \
                                update_date                                 \
                            ) values (                                      \
                                '" + ui->le_admin_homePhone->text() + "',   \
                                '1',                                        \
                                '" + QString::number(id_user) + "',         \
                                '" + update_date + "',                      \
                                '" + QString::number(id_user) + "',         \
                                '" + update_date + "'                       \
                            );"
        );
        QString new_office_address_id = database.read(db, "SELECT AUTO_INCREMENT            \
                                                           FROM  INFORMATION_SCHEMA.TABLES  \
                                                           WHERE TABLE_NAME   = 'address'").at(0);
        database.write(db, "insert into address (                                       \
                                address,                                                \
                                type_id,                                                \
                                create_by,                                              \
                                create_date,                                            \
                                update_by,                                              \
                                update_date                                             \
                            ) values (                                                  \
                                '" + ui->te_admin_officeAddress->toPlainText() + "',    \
                                '2',                                                    \
                                '" + QString::number(id_user) + "',                     \
                                '" + update_date + "',                                  \
                                '" + QString::number(id_user) + "',                     \
                                '" + update_date + "'                                   \
                            );"
        );
        QString new_office_phone_id = database.read(db, "SELECT AUTO_INCREMENT              \
                                                         FROM  INFORMATION_SCHEMA.TABLES    \
                                                         WHERE TABLE_NAME   = 'phone'").at(0);
        database.write(db, "insert into phone (                             \
                                number,                                     \
                                type_id,                                    \
                                create_by,                                  \
                                create_date,                                \
                                update_by,                                  \
                                update_date                                 \
                            ) values (                                      \
                                '" + ui->le_admin_officePhone->text() + "', \
                                '2',                                        \
                                '" + QString::number(id_user) + "',         \
                                '" + update_date + "',                      \
                                '" + QString::number(id_user) + "',         \
                                '" + update_date + "'                       \
                            );"
        );
        QString new_ident_number_id = database.read(db, "SELECT AUTO_INCREMENT              \
                                                         FROM  INFORMATION_SCHEMA.TABLES    \
                                                         WHERE TABLE_NAME   = 'number'").at(0);
        database.write(db, "insert into number (                            \
                                number,                                     \
                                type_id,                                    \
                                create_by,                                  \
                                create_date,                                \
                                update_by,                                  \
                                update_date                                 \
                            ) values (                                      \
                                '" + ui->le_admin_identNumber->text() + "', \
                                '1',                                        \
                                '" + QString::number(id_user) + "',         \
                                '" + update_date + "',                      \
                                '" + QString::number(id_user) + "',         \
                                '" + update_date + "'                       \
                            );"
        );
        QString new_identitifacion_id = database.read(db, "SELECT AUTO_INCREMENT            \
                                                           FROM  INFORMATION_SCHEMA.TABLES  \
                                                           WHERE TABLE_NAME   = 'identification'").at(0);
        database.write(db, "insert into identification (                                            \
                                name,                                                               \
                                ident_number_id,                                                    \
                                birth_place,                                                        \
                                birth_date,                                                         \
                                home_address_id,                                                    \
                                home_phone_id,                                                      \
                                office_address_id,                                                  \
                                office_phone_id,                                                    \
                                create_by,                                                          \
                                create_date,                                                        \
                                update_by,                                                          \
                                update_date                                                         \
                            ) values (                                                              \
                                '" + ui->le_admin_fullName->text() + "',                            \
                                '" + new_ident_number_id + "',                                      \
                                '" + ui->le_admin_birthPlace->text() + "',                          \
                                '" + ui->de_admin_birthDate->date().toString("dd-MM-yyyy") + "',    \
                                '" + new_home_address_id + "',                                      \
                                '" + new_home_phone_id + "',                                        \
                                '" + new_office_address_id + "',                                    \
                                '" + new_office_phone_id + "',                                      \
                                '" + QString::number(id_user) + "',                                 \
                                '" + update_date + "',                                              \
                                '" + QString::number(id_user) + "',                                 \
                                '" + update_date + "'                                               \
                            );"
        );
        database.write(db, "insert into user (                                              \
                                username,                                                   \
                                password,                                                   \
                                type_id,                                                    \
                                identification_id,                                          \
                                create_by,                                                  \
                                create_date,                                                \
                                update_by,                                                  \
                                update_date                                                 \
                            ) values (                                                      \
                                '" + ui->le_admin_username->text() + "',                    \
                                '" + ui->le_admin_password->text() + "', \
                                '2',                                                        \
                                '" + new_identitifacion_id + "',                            \
                                '" + QString::number(id_user) + "',                         \
                                '" + update_date + "',                                      \
                                '" + QString::number(id_user) + "',                         \
                                '" + update_date + "'                                       \
                            );"
        );
    }
    this->close();
}

void Form_Pendaftaran::on_pb_admin_cancel_clicked()
{
    this->close();
}

void Form_Pendaftaran::on_pb_assesor_ok_clicked()
{
    QString update_date = QString::number(QDateTime::currentDateTime().toTime_t());
    if (edit_mode) {
        database.write(db, "update user                                                                     \
                            set                                                                             \
                                password = '" + ui->le_assesor_password->text() + "',    \
                                update_by = '" + QString::number(id_user) + "',                             \
                                update_date = '" + update_date + "'                                         \
                            where id = '"+ QString::number(id_target) +"';"
                );
        database.write(db, "update identification                                                               \
                            set                                                                                 \
                                name = '" + ui->le_assesor_fullName->text() + "',                               \
                                birth_place = '" + ui->le_assesor_birthPlace->text() + "',                      \
                                birth_date = '" + ui->de_assesor_birthDate->date().toString("dd-MM-yyyy") + "', \
                                update_by = '" + QString::number(id_user) + "',                                 \
                                update_date = '" + update_date + "'                                             \
                            where id = (select                                                                  \
                                            identification_id                                                   \
                                        from                                                                    \
                                            user                                                                \
                                        where                                                                   \
                                            id = '"+ QString::number(id_target) +"');"
                );
        database.write(db, "update number                                                       \
                            set                                                                 \
                                number = '" + ui->le_assesor_identNumber->text() + "',          \
                                update_by = '" + QString::number(id_user) + "',                 \
                                update_date = '" + update_date + "'                             \
                            where id = (select                                                  \
                                            ident_number_id                                     \
                                        from                                                    \
                                            identification                                      \
                                        where                                                   \
                                            id = (select                                        \
                                                    identification_id                           \
                                                  from                                          \
                                                    user                                        \
                                                  where                                         \
                                                    id = '"+ QString::number(id_target) +"')    \
                                       );"
                );
        database.write(db, "update address                                                      \
                            set                                                                 \
                                address = '" + ui->te_assesor_homeAddress->toPlainText() + "',  \
                                update_by = '" + QString::number(id_user) + "',                 \
                                update_date = '" + update_date + "'                             \
                            where id = (select                                                  \
                                            home_address_id                                     \
                                        from                                                    \
                                            identification                                      \
                                        where                                                   \
                                            id = (select                                        \
                                                    identification_id                           \
                                                  from                                          \
                                                    user                                        \
                                                  where                                         \
                                                    id = '"+ QString::number(id_target) +"')    \
                                       );"
                );
        database.write(db, "update phone                                                        \
                            set                                                                 \
                                number = '" + ui->le_assesor_homePhone->text() + "',            \
                                update_by = '" + QString::number(id_user) + "',                 \
                                update_date = '" + update_date + "'                             \
                            where id = (select                                                  \
                                            home_phone_id                                       \
                                        from                                                    \
                                            identification                                      \
                                        where                                                   \
                                            id = (select                                        \
                                                    identification_id                           \
                                                  from                                          \
                                                    user                                        \
                                                  where                                         \
                                                    id = '"+ QString::number(id_target) +"')    \
                                       );"
                );
        database.write(db, "update address                                                          \
                            set                                                                     \
                                address = '" + ui->te_assesor_officeAddress->toPlainText() + "',    \
                                update_by = '" + QString::number(id_user) + "',                     \
                                update_date = '" + update_date + "'                                 \
                            where id = (select                                                      \
                                            office_address_id                                       \
                                        from                                                        \
                                            identification                                          \
                                        where                                                       \
                                            id = (select                                            \
                                                    identification_id                               \
                                                  from                                              \
                                                    user                                            \
                                                  where                                             \
                                                    id = '"+ QString::number(id_target) +"')        \
                                       );"
                );
        database.write(db, "update phone                                                        \
                            set                                                                 \
                                number = '" + ui->le_assesor_officePhone->text() + "',          \
                                update_by = '" + QString::number(id_user) + "',                 \
                                update_date = '" + update_date + "'                             \
                            where id = (select                                                  \
                                            office_phone_id                                     \
                                        from                                                    \
                                            identification                                      \
                                        where                                                   \
                                            id = (select                                        \
                                                    identification_id                           \
                                                  from                                          \
                                                    user                                        \
                                                  where                                         \
                                                    id = '"+ QString::number(id_target) +"')    \
                                       );"
                );
    } else {
        QString new_home_address_id = database.read(db, "SELECT AUTO_INCREMENT              \
                                                         FROM  INFORMATION_SCHEMA.TABLES    \
                                                         WHERE TABLE_NAME   = 'address'").at(0);
        database.write(db, "insert into address (                                       \
                                address,                                                \
                                type_id,                                                \
                                create_by,                                              \
                                create_date,                                            \
                                update_by,                                              \
                                update_date                                             \
                            ) values (                                                  \
                                '" + ui->te_assesor_homeAddress->toPlainText() + "',    \
                                '1',                                                    \
                                '" + QString::number(id_user) + "',                     \
                                '" + update_date + "',                                  \
                                '" + QString::number(id_user) + "',                     \
                                '" + update_date + "'                                   \
                            );"
        );
        QString new_home_phone_id = database.read(db, "SELECT AUTO_INCREMENT            \
                                                       FROM  INFORMATION_SCHEMA.TABLES  \
                                                       WHERE TABLE_NAME   = 'phone'").at(0);
        database.write(db, "insert into phone (                             \
                                number,                                     \
                                type_id,                                    \
                                create_by,                                  \
                                create_date,                                \
                                update_by,                                  \
                                update_date                                 \
                            ) values (                                      \
                                '" + ui->le_assesor_homePhone->text() + "', \
                                '1',                                        \
                                '" + QString::number(id_user) + "',         \
                                '" + update_date + "',                      \
                                '" + QString::number(id_user) + "',         \
                                '" + update_date + "'                       \
                            );"
        );
        QString new_office_address_id = database.read(db, "SELECT AUTO_INCREMENT            \
                                                           FROM  INFORMATION_SCHEMA.TABLES  \
                                                           WHERE TABLE_NAME   = 'address'").at(0);
        database.write(db, "insert into address (                                       \
                                address,                                                \
                                type_id,                                                \
                                create_by,                                              \
                                create_date,                                            \
                                update_by,                                              \
                                update_date                                             \
                            ) values (                                                  \
                                '" + ui->te_assesor_officeAddress->toPlainText() + "',  \
                                '2',                                                    \
                                '" + QString::number(id_user) + "',                     \
                                '" + update_date + "',                                  \
                                '" + QString::number(id_user) + "',                     \
                                '" + update_date + "'                                   \
                            );"
        );
        QString new_office_phone_id = database.read(db, "SELECT AUTO_INCREMENT              \
                                                         FROM  INFORMATION_SCHEMA.TABLES    \
                                                         WHERE TABLE_NAME   = 'phone'").at(0);
        database.write(db, "insert into phone (                                 \
                                number,                                         \
                                type_id,                                        \
                                create_by,                                      \
                                create_date,                                    \
                                update_by,                                      \
                                update_date                                     \
                            ) values (                                          \
                                '" + ui->le_assesor_officePhone->text() + "',   \
                                '2',                                            \
                                '" + QString::number(id_user) + "',             \
                                '" + update_date + "',                          \
                                '" + QString::number(id_user) + "',             \
                                '" + update_date + "'                           \
                            );"
        );
        QString new_ident_number_id = database.read(db, "SELECT AUTO_INCREMENT              \
                                                         FROM  INFORMATION_SCHEMA.TABLES    \
                                                         WHERE TABLE_NAME   = 'number'").at(0);
        database.write(db, "insert into number (                                \
                                number,                                         \
                                type_id,                                        \
                                create_by,                                      \
                                create_date,                                    \
                                update_by,                                      \
                                update_date                                     \
                            ) values (                                          \
                                '" + ui->le_assesor_identNumber->text() + "',   \
                                '1',                                            \
                                '" + QString::number(id_user) + "',             \
                                '" + update_date + "',                          \
                                '" + QString::number(id_user) + "',             \
                                '" + update_date + "'                           \
                            );"
        );
        QString new_identitifacion_id = database.read(db, "SELECT AUTO_INCREMENT            \
                                                           FROM  INFORMATION_SCHEMA.TABLES  \
                                                           WHERE TABLE_NAME   = 'identification'").at(0);
        database.write(db, "insert into identification (                                            \
                                name,                                                               \
                                ident_number_id,                                                    \
                                birth_place,                                                        \
                                birth_date,                                                         \
                                home_address_id,                                                    \
                                home_phone_id,                                                      \
                                office_address_id,                                                  \
                                office_phone_id,                                                    \
                                create_by,                                                          \
                                create_date,                                                        \
                                update_by,                                                          \
                                update_date                                                         \
                            ) values (                                                              \
                                '" + ui->le_assesor_fullName->text() + "',                          \
                                '" + new_ident_number_id + "',                                      \
                                '" + ui->le_assesor_birthPlace->text() + "',                        \
                                '" + ui->de_assesor_birthDate->date().toString("dd-MM-yyyy") + "',  \
                                '" + new_home_address_id + "',                                      \
                                '" + new_home_phone_id + "',                                        \
                                '" + new_office_address_id + "',                                    \
                                '" + new_office_phone_id + "',                                      \
                                '" + QString::number(id_user) + "',                                 \
                                '" + update_date + "',                                              \
                                '" + QString::number(id_user) + "',                                 \
                                '" + update_date + "'                                               \
                            );"
        );
        database.write(db, "insert into user (                                                  \
                                username,                                                       \
                                password,                                                       \
                                type_id,                                                        \
                                identification_id,                                              \
                                create_by,                                                      \
                                create_date,                                                    \
                                update_by,                                                      \
                                update_date                                                     \
                            ) values (                                                          \
                                '" + ui->le_assesor_username->text() + "',                      \
                                '" + ui->le_assesor_password->text() + "',   \
                                '3',                                                            \
                                '" + new_identitifacion_id + "',                                \
                                '" + QString::number(id_user) + "',                             \
                                '" + update_date + "',                                          \
                                '" + QString::number(id_user) + "',                             \
                                '" + update_date + "'                                           \
                            );"
        );
    }
    this->close();
}

void Form_Pendaftaran::on_pb_assesor_cancel_clicked()
{
    this->close();
}
