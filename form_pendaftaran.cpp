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

        ui->le_admin_username->setValidator(new QRegExpValidator(QRegExp("^[A-Za-z0-9]*$"), ui->le_admin_username));
        ui->le_admin_password->setValidator(new QRegExpValidator(QRegExp("^[A-Za-z0-9]*$"), ui->le_admin_password));
        ui->le_admin_identNumber->setValidator(new QRegExpValidator(QRegExp("^\\d{0,20}$"), ui->le_admin_identNumber));
        ui->le_admin_homePhone->setValidator(new QRegExpValidator(QRegExp("^\\d{0,13}$"), ui->le_admin_homePhone));
        ui->le_admin_officePhone->setValidator(new QRegExpValidator(QRegExp("^\\d{0,13}$"), ui->le_admin_officePhone));
    } else if (mode == 2) {
        ui->tabWidget->removeTab(2);
        ui->tabWidget->removeTab(0);

        ui->le_assessor_identNumber->setValidator(new QRegExpValidator(QRegExp("^\\d{0,20}$"), ui->le_assessor_identNumber));
        ui->le_assessor_homePhone->setValidator(new QRegExpValidator(QRegExp("^\\d{0,13}$"), ui->le_assessor_homePhone));
        ui->le_assessor_officePhone->setValidator(new QRegExpValidator(QRegExp("^\\d{0,13}$"), ui->le_assessor_officePhone));
    } else if (mode == 3) {
        ui->tabWidget->removeTab(1);
        ui->tabWidget->removeTab(0);

        ui->le_sertifikat_identNumber->setValidator(new QRegExpValidator(QRegExp("^\\d{0,20}$"), ui->le_sertifikat_identNumber));
        ui->le_sertifikat_homePhone->setValidator(new QRegExpValidator(QRegExp("^\\d{0,13}$"), ui->le_sertifikat_homePhone));
        ui->le_sertifikat_officePhone->setValidator(new QRegExpValidator(QRegExp("^\\d{0,13}$"), ui->le_sertifikat_officePhone));
//        setCompetence();
        setAssessor();
    }
}

//void Form_Pendaftaran::setCompetence()
//{
//    QStringList result = database.read(db,"select id, name from competence;");
//    if (result.isEmpty()) {
//        QMessageBox::warning(this, "Form - LSPE", "Competence is Empty ..");
//        return;
//    }
//    competence.clear();
//    for (int i = 0; i < result.length(); i+=2) {
//        competence.append(result.at(i));
//        competence.append(result.at(i+1));
//        ui->cb_sertifikat_competence->addItem(result.at(i+1));
//    }
//}

void Form_Pendaftaran::setAssessor()
{
    QStringList result = database.read(db,
                                  "select                               \
                                       a.id,                            \
                                       i.name                           \
                                   from                                 \
                                       assessor as a,                   \
                                       identification as i              \
                                   where                                \
                                       a.identification_id = i.id and   \
                                       a.status = 1                     \
                                   order by i.name;"
            );
    if (result.isEmpty()) {
        QMessageBox::warning(this, "Form - LSPE", "Please add some Assessor for continue this process ..");
        this->close();
    }
    assessor.clear();
    for (int i = 0; i < result.length(); i+=2) {
        assessor.append(result.at(i));
        assessor.append(result.at(i+1));
        ui->cb_sertifikat_assessor->addItem(result.at(i+1));
    }
}

void Form_Pendaftaran::setEdit(bool edit, int id, int mode)
{
    if (edit) {
        if (mode == 1) {
            QStringList result = database.read(db,
                                          "select                                                                                   \
                                               u.username,                                                                          \
                                               i.name,                                                                              \
                                               i.birth_place,                                                                       \
                                               i.birth_date,                                                                        \
                                               i.gender,                                                                            \
                                               (select number from number where id = i.ident_number_id) as identification_number,   \
                                               (select address from address where id = i.home_address_id) as home_address,          \
                                               (select number from phone where id = i.home_phone_id) as home_phone_number,          \
                                               (select address from address where id = i.office_address_id) as office_address,      \
                                               (select number from phone where id = i.office_phone_id) as office_phone_number,      \
                                               (select address from address where id = i.email_address_id) as email_address         \
                                           from                                                                                     \
                                               user as u,                                                                           \
                                               identification as i                                                                  \
                                           where                                                                                    \
                                               u.identification_id = i.id and                                                       \
                                               u.id = " + QString::number(id)
                    );

            ui->le_admin_username->setText(result.at(0));
            ui->le_admin_username->setEnabled(false);
            ui->le_admin_fullName->setText(result.at(1));
            ui->le_admin_birthPlace->setText(result.at(2));
            ui->de_admin_birthDate->setDate(QDate::fromString(result.at(3), "dd-MM-yyyy"));
            ui->cb_admin_gender->setCurrentText(result.at(4));
            ui->le_admin_identNumber->setText(result.at(5));
            ui->te_admin_homeAddress->setText(result.at(6));
            ui->le_admin_homePhone->setText(result.at(7));
            ui->te_admin_officeAddress->setText(result.at(8));
            ui->le_admin_officePhone->setText(result.at(9));
            ui->le_admin_emailAddress->setText(result.at(10));
        } else if (mode == 2) {
            QStringList result = database.read(db,
                                          "select                                                                                   \
                                               i.name,                                                                              \
                                               i.birth_place,                                                                       \
                                               i.birth_date,                                                                        \
                                               i.gender,                                                                             \
                                               (select number from number where id = i.ident_number_id) as identification_number,   \
                                               (select address from address where id = i.home_address_id) as home_address,          \
                                               (select number from phone where id = i.home_phone_id) as home_phone_number,          \
                                               (select address from address where id = i.office_address_id) as office_address,      \
                                               (select number from phone where id = i.office_phone_id) as office_phone_number,      \
                                               (select address from address where id = i.email_address_id) as email_address         \
                                           from                                                                                     \
                                               assessor as a,                                                                        \
                                               identification as i                                                                  \
                                           where                                                                                    \
                                               a.identification_id = i.id and                                                       \
                                               a.id = " + QString::number(id)
                    );

            ui->le_assessor_fullName->setText(result.at(0));
            ui->le_assessor_birthPlace->setText(result.at(1));
            ui->de_assessor_birthDate->setDate(QDate::fromString(result.at(2), "dd-MM-yyyy"));
            ui->cb_assessor_gender->setCurrentText(result.at(3));
            ui->le_assessor_identNumber->setText(result.at(4));
            ui->te_assessor_homeAddress->setText(result.at(5));
            ui->le_assessor_homePhone->setText(result.at(6));
            ui->te_assessor_officeAddress->setText(result.at(7));
            ui->le_assessor_officePhone->setText(result.at(8));
            ui->le_assessor_emailAddress->setText(result.at(9));
        } else if (mode == 3) {
            QStringList result = database.read(db,
                                          "select                                                                                           \
                                               (select name from identification where id = c.identification_id) as full_name,               \
                                               (select path from picture where id = c.photo_picture_id) as photo_picture,                   \
                                               (select number from number where id = (                                                      \
                                                   select ident_number_id from identification where id = c.identification_id                \
                                               )) as identification_number,                                                                 \
                                               (select birth_place from identification where id = c.identification_id) as birth_place,      \
                                               (select birth_date from identification where id = c.identification_id) as birth_date,        \
                                               (select gender from identification where id = c.identification_id) as gender,                \
                                               (select address from address where id = (                                                    \
                                                   select home_address_id from identification where id = c.identification_id                \
                                               )) as home_address,                                                                          \
                                               (select number from phone where id = (                                                       \
                                                   select home_phone_id from identification where id = c.identification_id                  \
                                               )) as home_phone,                                                                            \
                                               (select address from address where id = (                                                    \
                                                   select office_address_id from identification where id = c.identification_id              \
                                               )) as office_address,                                                                        \
                                               (select number from phone where id = (                                                       \
                                                   select office_phone_id from identification where id = c.identification_id                \
                                               )) as office_phone,                                                                          \
                                               (select address from address where id = (                                                    \
                                                   select email_address_id from identification where id = c.identification_id               \
                                               )) as email_address,                                                                         \
                                               (select number from rfid where id = c.rfid) as rfid,                                         \
                                               (select number from number where id = c.cert_number_id) as certification_number,             \
                                               (select path from picture where id = c.certification_picture_id) as certification_picture,   \
                                               (select number from number where id = c.reg_number_id) as registration_number,               \
                                               (select number from number where id = c.kbji_number_id) as kbji_number,                      \
                                               c.competence,                                                                                \
                                               c.start_date,                                                                                \
                                               c.end_date,                                                                                  \
                                               (select name from identification where id = (                                                \
                                                   select identification_id from assessor where id = c.assessor_id                          \
                                               )) as assessor_name,                                                                         \
                                               (select address from address where id = c.assessment_place_id) as assessment_place,          \
                                               c.assessment_date                                                                            \
                                           from                                                                                             \
                                               certification c                                                                              \
                                           where                                                                                            \
                                               id = " + QString::number(id)
                    );

            ui->le_sertifikat_fullName->setText(result.at(0));
            ui->le_sertifikat_photoFile->setText(result.at(1));
            ui->le_sertifikat_identNumber->setText(result.at(2));
            ui->le_sertifikat_birthPlace->setText(result.at(3));
            ui->de_sertifikat_birthDate->setDate(QDate::fromString(result.at(4), "dd-MM-yyyy"));
            ui->cb_sertifikat_gender->setCurrentText(result.at(5));
            ui->te_sertifikat_homeAddress->setText(result.at(6));
            ui->le_sertifikat_homePhone->setText(result.at(7));
            ui->te_sertifikat_officeAddress->setText(result.at(8));
            ui->le_sertifikat_officePhone->setText(result.at(9));
            ui->le_sertifikat_emailAddress->setText(result.at(10));
            ui->le_sertifikat_rfid->setText(result.at(11));
            ui->le_sertifikat_certNumber->setText(result.at(12));
            ui->le_sertifikat_certFile->setText(result.at(13));
            ui->le_sertifikat_registrationNumber->setText(result.at(14));
            ui->le_sertifikat_kbjiNumber->setText(result.at(15));
            ui->le_sertifikat_competence->setText(result.at(16));
            ui->de_sertifikat_startDate->setDate(QDate::fromString(result.at(17), "dd-MM-yyyy"));
            ui->de_sertifikat_endDate->setDate(QDate::fromString(result.at(18), "dd-MM-yyyy"));
            ui->cb_sertifikat_assessor->setCurrentText(result.at(19));
            ui->te_sertifikat_assessmentPlace->setText(result.at(20));
            ui->de_sertifikat_assessmentDate->setDate(QDate::fromString(result.at(21), "dd-MM-yyyy"));

        }
    }
}

void Form_Pendaftaran::on_pb_admin_ok_clicked()
{
    if (validationAdminForm()) {
        QString update_date = QString::number(QDateTime::currentDateTime().toTime_t());
        if (edit_mode) {
            if (!ui->le_admin_password->text().isEmpty()) {
                QString encodedPass = QString(QCryptographicHash::hash(ui->le_admin_password->text().toUtf8(),QCryptographicHash::Md5).toHex());
                database.write(db, "update user                                                                 \
                                    set                                                                         \
                                        password = '" + encodedPass + "',                                       \
                                        update_by = '" + QString::number(id_user) + "',                         \
                                        update_date = '" + update_date + "'                                     \
                                    where id = '"+ QString::number(id_target) +"';"
                        );
            }
            database.write(db, "update identification                                                               \
                                set                                                                                 \
                                    name = '" + ui->le_admin_fullName->text() + "',                                 \
                                    birth_place = '" + ui->le_admin_birthPlace->text() + "',                        \
                                    birth_date = '" + ui->de_admin_birthDate->date().toString("dd-MM-yyyy") + "',   \
                                    gender = '"+ui->cb_admin_gender->currentText()+"',                              \
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
            database.write(db, "update address                                                                          \
                                set                                                                                     \
                                    address = '" + ui->te_admin_homeAddress->toPlainText() + "',                        \
                                    update_by = '" + QString::number(id_user) + "',                                     \
                                    update_date = '" + update_date + "'                                                 \
                                where id = (select                                                                      \
                                                home_address_id                                                         \
                                            from                                                                        \
                                                identification                                                          \
                                            where                                                                       \
                                                id = (select                                                            \
                                                        identification_id                                               \
                                                      from                                                              \
                                                        user                                                            \
                                                      where                                                             \
                                                        id = '"+ QString::number(id_target) +"')                        \
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
            database.write(db, "update address                                                                          \
                                set                                                                                     \
                                    address = '" + ui->le_admin_emailAddress->text() + "',                              \
                                    update_by = '" + QString::number(id_user) + "',                                     \
                                    update_date = '" + update_date + "'                                                 \
                                where id = (select                                                                      \
                                                email_address_id                                                        \
                                            from                                                                        \
                                                identification                                                          \
                                            where                                                                       \
                                                id = (select                                                            \
                                                        identification_id                                               \
                                                      from                                                              \
                                                        user                                                            \
                                                      where                                                             \
                                                        id = '"+ QString::number(id_target) +"')                        \
                                           );"
                    );
        } else {
            QString new_home_address_id = database.read(db, "SELECT AUTO_INCREMENT              \
                                                             FROM  INFORMATION_SCHEMA.TABLES    \
                                                             WHERE TABLE_NAME  = 'address'").at(0);
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
                                                           WHERE TABLE_NAME  = 'phone'").at(0);
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
                                                               WHERE TABLE_NAME  = 'address'").at(0);
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
                                                             WHERE TABLE_NAME  = 'phone'").at(0);
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
            QString new_email_address_id = database.read(db, "SELECT AUTO_INCREMENT              \
                                                              FROM  INFORMATION_SCHEMA.TABLES    \
                                                              WHERE TABLE_NAME  = 'address'").at(0);
            database.write(db, "insert into address (                                   \
                                    address,                                            \
                                    type_id,                                            \
                                    create_by,                                          \
                                    create_date,                                        \
                                    update_by,                                          \
                                    update_date                                         \
                                ) values (                                              \
                                    '" + ui->le_admin_emailAddress->text() + "',        \
                                    '3',                                                \
                                    '" + QString::number(id_user) + "',                 \
                                    '" + update_date + "',                              \
                                    '" + QString::number(id_user) + "',                 \
                                    '" + update_date + "'                               \
                                );"
            );
            QString new_ident_number_id = database.read(db, "SELECT AUTO_INCREMENT              \
                                                             FROM  INFORMATION_SCHEMA.TABLES    \
                                                             WHERE TABLE_NAME  = 'number'").at(0);
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
                                                               WHERE TABLE_NAME  = 'identification'").at(0);
            database.write(db, "insert into identification (                                            \
                                    name,                                                               \
                                    ident_number_id,                                                    \
                                    birth_place,                                                        \
                                    birth_date,                                                         \
                                    gender,                                                             \
                                    home_address_id,                                                    \
                                    home_phone_id,                                                      \
                                    office_address_id,                                                  \
                                    office_phone_id,                                                    \
                                    email_address_id,                                                   \
                                    create_by,                                                          \
                                    create_date,                                                        \
                                    update_by,                                                          \
                                    update_date                                                         \
                                ) values (                                                              \
                                    '" + ui->le_admin_fullName->text() + "',                            \
                                    '" + new_ident_number_id + "',                                      \
                                    '" + ui->le_admin_birthPlace->text() + "',                          \
                                    '" + ui->de_admin_birthDate->date().toString("dd-MM-yyyy") + "',    \
                                    '" + ui->cb_admin_gender->currentText() + "',                       \
                                    '" + new_home_address_id + "',                                      \
                                    '" + new_home_phone_id + "',                                        \
                                    '" + new_office_address_id + "',                                    \
                                    '" + new_office_phone_id + "',                                      \
                                    '" + new_email_address_id + "',                                     \
                                    '" + QString::number(id_user) + "',                                 \
                                    '" + update_date + "',                                              \
                                    '" + QString::number(id_user) + "',                                 \
                                    '" + update_date + "'                                               \
                                );"
            );
            QString encodedPass = QString(QCryptographicHash::hash(ui->le_admin_password->text().toUtf8(),QCryptographicHash::Md5).toHex());
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
                                    '" + encodedPass + "',                                      \
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
    } else {
        QMessageBox::warning(this, "Form - LSPE", "Please complete the form ..");
    }
}

void Form_Pendaftaran::on_pb_admin_cancel_clicked()
{
    this->close();
}

void Form_Pendaftaran::on_pb_assessor_ok_clicked()
{
    if (validationassessorForm()) {
        QString update_date = QString::number(QDateTime::currentDateTime().toTime_t());
        if (edit_mode) {
            database.write(db, "update identification                                                               \
                                set                                                                                 \
                                    name = '" + ui->le_assessor_fullName->text() + "',                               \
                                    birth_place = '" + ui->le_assessor_birthPlace->text() + "',                      \
                                    birth_date = '" + ui->de_assessor_birthDate->date().toString("dd-MM-yyyy") + "', \
                                    gender = '" + ui->cb_assessor_gender->currentText() + "',                        \
                                    update_by = '" + QString::number(id_user) + "',                                 \
                                    update_date = '" + update_date + "'                                             \
                                where id = (select                                                                  \
                                                identification_id                                                   \
                                            from                                                                    \
                                                assessor                                                                \
                                            where                                                                   \
                                                id = '"+ QString::number(id_target) +"');"
                    );
            database.write(db, "update number                                                       \
                                set                                                                 \
                                    number = '" + ui->le_assessor_identNumber->text() + "',          \
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
                                                        assessor                                        \
                                                      where                                         \
                                                        id = '"+ QString::number(id_target) +"')    \
                                           );"
                    );
            database.write(db, "update address                                                      \
                                set                                                                 \
                                    address = '" + ui->te_assessor_homeAddress->toPlainText() + "',  \
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
                                                        assessor                                        \
                                                      where                                         \
                                                        id = '"+ QString::number(id_target) +"')    \
                                           );"
                    );
            database.write(db, "update phone                                                        \
                                set                                                                 \
                                    number = '" + ui->le_assessor_homePhone->text() + "',            \
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
                                                        assessor                                        \
                                                      where                                         \
                                                        id = '"+ QString::number(id_target) +"')    \
                                           );"
                    );
            database.write(db, "update address                                                          \
                                set                                                                     \
                                    address = '" + ui->te_assessor_officeAddress->toPlainText() + "',    \
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
                                                        assessor                                            \
                                                      where                                             \
                                                        id = '"+ QString::number(id_target) +"')        \
                                           );"
                    );
            database.write(db, "update phone                                                        \
                                set                                                                 \
                                    number = '" + ui->le_assessor_officePhone->text() + "',         \
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
                                                        assessor                                        \
                                                      where                                         \
                                                        id = '"+ QString::number(id_target) +"')    \
                                           );"
                    );
            database.write(db, "update address                                                      \
                                set                                                                 \
                                    address = '" + ui->le_assessor_emailAddress->text() + "',       \
                                    update_by = '" + QString::number(id_user) + "',                 \
                                    update_date = '" + update_date + "'                             \
                                where id = (select                                                  \
                                                email_address_id                                    \
                                            from                                                    \
                                                identification                                      \
                                            where                                                   \
                                                id = (select                                        \
                                                        identification_id                           \
                                                      from                                          \
                                                        assessor                                        \
                                                      where                                         \
                                                        id = '"+ QString::number(id_target) +"')    \
                                           );"
                    );
        } else {
            QString new_home_address_id = database.read(db, "SELECT AUTO_INCREMENT              \
                                                             FROM  INFORMATION_SCHEMA.TABLES    \
                                                             WHERE TABLE_NAME  = 'address'").at(0);
            database.write(db, "insert into address (                                       \
                                    address,                                                \
                                    type_id,                                                \
                                    create_by,                                              \
                                    create_date,                                            \
                                    update_by,                                              \
                                    update_date                                             \
                                ) values (                                                  \
                                    '" + ui->te_assessor_homeAddress->toPlainText() + "',   \
                                    '1',                                                    \
                                    '" + QString::number(id_user) + "',                     \
                                    '" + update_date + "',                                  \
                                    '" + QString::number(id_user) + "',                     \
                                    '" + update_date + "'                                   \
                                );"
            );
            QString new_home_phone_id = database.read(db, "SELECT AUTO_INCREMENT            \
                                                           FROM  INFORMATION_SCHEMA.TABLES  \
                                                           WHERE TABLE_NAME  = 'phone'").at(0);
            database.write(db, "insert into phone (                                 \
                                    number,                                         \
                                    type_id,                                        \
                                    create_by,                                      \
                                    create_date,                                    \
                                    update_by,                                      \
                                    update_date                                     \
                                ) values (                                          \
                                    '" + ui->le_assessor_homePhone->text() + "',    \
                                    '1',                                            \
                                    '" + QString::number(id_user) + "',             \
                                    '" + update_date + "',                          \
                                    '" + QString::number(id_user) + "',             \
                                    '" + update_date + "'                           \
                                );"
            );
            QString new_office_address_id = database.read(db, "SELECT AUTO_INCREMENT            \
                                                               FROM  INFORMATION_SCHEMA.TABLES  \
                                                               WHERE TABLE_NAME  = 'address'").at(0);
            database.write(db, "insert into address (                                       \
                                    address,                                                \
                                    type_id,                                                \
                                    create_by,                                              \
                                    create_date,                                            \
                                    update_by,                                              \
                                    update_date                                             \
                                ) values (                                                  \
                                    '" + ui->te_assessor_officeAddress->toPlainText() + "', \
                                    '2',                                                    \
                                    '" + QString::number(id_user) + "',                     \
                                    '" + update_date + "',                                  \
                                    '" + QString::number(id_user) + "',                     \
                                    '" + update_date + "'                                   \
                                );"
            );
            QString new_office_phone_id = database.read(db, "SELECT AUTO_INCREMENT              \
                                                             FROM  INFORMATION_SCHEMA.TABLES    \
                                                             WHERE TABLE_NAME  = 'phone'").at(0);
            database.write(db, "insert into phone (                                 \
                                    number,                                         \
                                    type_id,                                        \
                                    create_by,                                      \
                                    create_date,                                    \
                                    update_by,                                      \
                                    update_date                                     \
                                ) values (                                          \
                                    '" + ui->le_assessor_officePhone->text() + "',  \
                                    '2',                                            \
                                    '" + QString::number(id_user) + "',             \
                                    '" + update_date + "',                          \
                                    '" + QString::number(id_user) + "',             \
                                    '" + update_date + "'                           \
                                );"
            );
            QString new_email_address_id = database.read(db, "SELECT AUTO_INCREMENT            \
                                                              FROM  INFORMATION_SCHEMA.TABLES  \
                                                              WHERE TABLE_NAME  = 'address'").at(0);
            database.write(db, "insert into address (                               \
                                    address,                                        \
                                    type_id,                                        \
                                    create_by,                                      \
                                    create_date,                                    \
                                    update_by,                                      \
                                    update_date                                     \
                                ) values (                                          \
                                    '" + ui->le_assessor_emailAddress->text() + "', \
                                    '3',                                            \
                                    '" + QString::number(id_user) + "',             \
                                    '" + update_date + "',                          \
                                    '" + QString::number(id_user) + "',             \
                                    '" + update_date + "'                           \
                                );"
            );
            QString new_ident_number_id = database.read(db, "SELECT AUTO_INCREMENT              \
                                                             FROM  INFORMATION_SCHEMA.TABLES    \
                                                             WHERE TABLE_NAME  = 'number'").at(0);
            database.write(db, "insert into number (                                \
                                    number,                                         \
                                    type_id,                                        \
                                    create_by,                                      \
                                    create_date,                                    \
                                    update_by,                                      \
                                    update_date                                     \
                                ) values (                                          \
                                    '" + ui->le_assessor_identNumber->text() + "',  \
                                    '1',                                            \
                                    '" + QString::number(id_user) + "',             \
                                    '" + update_date + "',                          \
                                    '" + QString::number(id_user) + "',             \
                                    '" + update_date + "'                           \
                                );"
            );
            QString new_identitifacion_id = database.read(db, "SELECT AUTO_INCREMENT            \
                                                               FROM  INFORMATION_SCHEMA.TABLES  \
                                                               WHERE TABLE_NAME  = 'identification'").at(0);
            database.write(db, "insert into identification (                                            \
                                    name,                                                               \
                                    ident_number_id,                                                    \
                                    birth_place,                                                        \
                                    birth_date,                                                         \
                                    gender,                                                             \
                                    home_address_id,                                                    \
                                    home_phone_id,                                                      \
                                    office_address_id,                                                  \
                                    office_phone_id,                                                    \
                                    email_address_id,                                                   \
                                    create_by,                                                          \
                                    create_date,                                                        \
                                    update_by,                                                          \
                                    update_date                                                         \
                                ) values (                                                              \
                                    '" + ui->le_assessor_fullName->text() + "',                          \
                                    '" + new_ident_number_id + "',                                      \
                                    '" + ui->le_assessor_birthPlace->text() + "',                        \
                                    '" + ui->de_assessor_birthDate->date().toString("dd-MM-yyyy") + "',  \
                                    '" + ui->cb_assessor_gender->currentText() + "',                     \
                                    '" + new_home_address_id + "',                                      \
                                    '" + new_home_phone_id + "',                                        \
                                    '" + new_office_address_id + "',                                    \
                                    '" + new_office_phone_id + "',                                      \
                                    '" + new_email_address_id + "',                                     \
                                    '" + QString::number(id_user) + "',                                 \
                                    '" + update_date + "',                                              \
                                    '" + QString::number(id_user) + "',                                 \
                                    '" + update_date + "'                                               \
                                );"
            );
            database.write(db, "insert into assessor (                                              \
                                    identification_id,                                              \
                                    create_by,                                                      \
                                    create_date,                                                    \
                                    update_by,                                                      \
                                    update_date                                                     \
                                ) values (                                                          \
                                    '" + new_identitifacion_id + "',                                \
                                    '" + QString::number(id_user) + "',                             \
                                    '" + update_date + "',                                          \
                                    '" + QString::number(id_user) + "',                             \
                                    '" + update_date + "'                                           \
                                );"
            );
        }
        this->close();
    } else {
        QMessageBox::warning(this, "Form - LSPE", "Please complete the form ..");
    }
}

void Form_Pendaftaran::on_pb_assessor_cancel_clicked()
{
    this->close();
}

void Form_Pendaftaran::on_pb_sertifikat_ok_clicked()
{
    if (validationSertifikatForm()) {
        QString update_date = QString::number(QDateTime::currentDateTime().toTime_t());
        if (edit_mode) {
            QString newPhotoName = "photo_" +
                    ui->le_sertifikat_identNumber->text() + "_" +
                    ui->le_sertifikat_fullName->text() + "_" +
                    ui->le_sertifikat_certNumber->text();
            this->save_file(ui->le_sertifikat_photoFile->text(), newPhotoName);
            QString newCertificateName = "certificate_"+
                    ui->le_sertifikat_identNumber->text() + "_" +
                    ui->le_sertifikat_fullName->text() + "_" +
                    ui->le_sertifikat_certNumber->text();
            this->save_file(ui->le_sertifikat_certFile->text(), newCertificateName);

            database.write(db, "update identification                                                                   \
                                set                                                                                     \
                                    name = '" + ui->le_sertifikat_fullName->text() + "',                                \
                                    birth_place = '" + ui->le_sertifikat_birthPlace->text() + "',                       \
                                    birth_date = '" + ui->de_sertifikat_birthDate->date().toString("dd-MM-yyyy") + "',  \
                                    gender = '" + ui->cb_sertifikat_gender->currentText() + "',                         \
                                    update_by = '" + QString::number(id_user) + "',                                     \
                                    update_date = '" + update_date + "'                                                 \
                                where id = (select                                                                      \
                                                identification_id                                                       \
                                            from                                                                        \
                                                certification                                                           \
                                            where                                                                       \
                                                id = '"+ QString::number(id_target) +"');"
                    );
            database.write(db, "update number                                                       \
                                set                                                                 \
                                    number = '" + ui->le_sertifikat_identNumber->text() + "',       \
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
                                                        certification                               \
                                                      where                                         \
                                                        id = '"+ QString::number(id_target) +"')    \
                                           );"
                    );
            database.write(db, "update address                                                          \
                                set                                                                     \
                                    address = '" + ui->te_sertifikat_homeAddress->toPlainText() + "',   \
                                    update_by = '" + QString::number(id_user) + "',                     \
                                    update_date = '" + update_date + "'                                 \
                                where id = (select                                                      \
                                                home_address_id                                         \
                                            from                                                        \
                                                identification                                          \
                                            where                                                       \
                                                id = (select                                            \
                                                        identification_id                               \
                                                      from                                              \
                                                        certification                                   \
                                                      where                                             \
                                                        id = '"+ QString::number(id_target) +"')        \
                                           );"
                    );
            database.write(db, "update phone                                                        \
                                set                                                                 \
                                    number = '" + ui->le_sertifikat_homePhone->text() + "',         \
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
                                                        certification                               \
                                                      where                                         \
                                                        id = '"+ QString::number(id_target) +"')    \
                                           );"
                    );
            database.write(db, "update address                                                          \
                                set                                                                     \
                                    address = '" + ui->te_sertifikat_officeAddress->toPlainText() + "', \
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
                                                        certification                                   \
                                                      where                                             \
                                                        id = '"+ QString::number(id_target) +"')        \
                                           );"
                    );
            database.write(db, "update phone                                                        \
                                set                                                                 \
                                    number = '" + ui->le_sertifikat_officePhone->text() + "',       \
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
                                                        certification                               \
                                                      where                                         \
                                                        id = '"+ QString::number(id_target) +"')    \
                                           );"
                    );
            database.write(db, "update address                                                      \
                                set                                                                 \
                                    address = '" + ui->le_sertifikat_emailAddress->text() + "',     \
                                    update_by = '" + QString::number(id_user) + "',                 \
                                    update_date = '" + update_date + "'                             \
                                where id = (select                                                  \
                                                email_address_id                                    \
                                            from                                                    \
                                                identification                                      \
                                            where                                                   \
                                                id = (select                                        \
                                                        identification_id                           \
                                                      from                                          \
                                                        certification                               \
                                                      where                                         \
                                                        id = '"+ QString::number(id_target) +"')    \
                                           );"
                    );
            database.write(db, "update picture                                              \
                                set                                                         \
                                    path = '" + newPhotoName + "',                          \
                                    update_by = '" + QString::number(id_user) + "',         \
                                    update_date = '" + update_date + "'                     \
                                where id = (select                                          \
                                                photo_picture_id                            \
                                            from                                            \
                                                certification                               \
                                            where                                           \
                                                id = '"+ QString::number(id_target) +"'     \
                                           );"
                    );
            database.write(db, "update picture                                              \
                                set                                                         \
                                    path = '" + newCertificateName + "',                    \
                                    update_by = '" + QString::number(id_user) + "',         \
                                    update_date = '" + update_date + "'                     \
                                where id = (select                                          \
                                                certification_picture_id                    \
                                            from                                            \
                                                certification                               \
                                            where                                           \
                                                id = '"+ QString::number(id_target) +"'     \
                                           );"
                    );
            database.write(db, "update number                                                           \
                                set                                                                     \
                                    number = '" + ui->le_sertifikat_registrationNumber->text() + "',    \
                                    update_by = '" + QString::number(id_user) + "',                     \
                                    update_date = '" + update_date + "'                                 \
                                where id = (select                                                        \
                                              reg_number_id                                             \
                                          from                                                          \
                                              certification                                             \
                                          where                                                         \
                                              id = '"+ QString::number(id_target) +"'                   \
                                          );"
            );
            database.write(db, "update number                                                   \
                                set                                                             \
                                    number = '" + ui->le_sertifikat_certNumber->text() + "',    \
                                    update_by = '" + QString::number(id_user) + "',             \
                                    update_date = '" + update_date + "'                         \
                                where id = (select                                                \
                                              cert_number_id                                    \
                                          from                                                  \
                                              certification                                     \
                                          where                                                 \
                                              id = '"+ QString::number(id_target) +"'           \
                                          );"
            );
            database.write(db, "update number                                                   \
                                set                                                             \
                                    number = '" + ui->le_sertifikat_kbjiNumber->text() + "',    \
                                    update_by = '" + QString::number(id_user) + "',             \
                                    update_date = '" + update_date + "'                         \
                                where id = (select                                                \
                                              kbji_number_id                                    \
                                          from                                                  \
                                              certification                                     \
                                          where                                                 \
                                              id = '"+ QString::number(id_target) +"'           \
                                          );"
            );
            database.write(db, "update rfid                                             \
                                set                                                     \
                                    number = '" + ui->le_sertifikat_rfid->text() + "',  \
                                    update_by = '" + QString::number(id_user) + "',     \
                                    update_date = '" + update_date + "'                 \
                                where id = (select                                        \
                                              rfid                                      \
                                          from                                          \
                                              certification                             \
                                          where                                         \
                                              id = '"+ QString::number(id_target) +"'   \
                                          );"
            );
            database.write(db, "update address                                                              \
                                set                                                                         \
                                    address = '" + ui->te_sertifikat_assessmentPlace->toPlainText() + "',   \
                                    update_by = '" + QString::number(id_user) + "',                         \
                                    update_date = '" + update_date + "'                                     \
                                where id = (select                                                          \
                                                assessment_place_id                                         \
                                            from                                                            \
                                                certification                                               \
                                            where                                                           \
                                                id = '"+ QString::number(id_target) +"'                     \
                                           );"
                    );
            database.write(db, "update certification                                                                    \
                                set                                                                                     \
                                    competence = '" + ui->le_sertifikat_competence->text() + "',                        \
                                    start_date = '" + ui->de_sertifikat_startDate->date().toString("dd-MM-yyyy") + "',  \
                                    end_date = '" + ui->de_sertifikat_endDate->date().toString("dd-MM-yyyy") + "',        \
                                    assessor_id = '" + assessor.at(ui->cb_sertifikat_assessor->currentIndex()*2) + "',  \
                                    update_by = '" + QString::number(id_user) + "',                                     \
                                    update_date = '" + update_date + "'                                                 \
                                where id = '"+ QString::number(id_target) +"';"
            );
        } else {
            QString newPhotoName = "photo_" +
                    ui->le_sertifikat_identNumber->text() + "_" +
                    ui->le_sertifikat_fullName->text() + "_" +
                    ui->le_sertifikat_certNumber->text();
            this->save_file(ui->le_sertifikat_photoFile->text(), newPhotoName);
            QString newCertificateName = "certificate_" +
                    ui->le_sertifikat_identNumber->text() + "_" +
                    ui->le_sertifikat_fullName->text() + "_" +
                    ui->le_sertifikat_certNumber->text();
            this->save_file(ui->le_sertifikat_certFile->text(), newCertificateName);

            QString new_home_address_id = database.read(db, "SELECT AUTO_INCREMENT              \
                                                             FROM  INFORMATION_SCHEMA.TABLES    \
                                                             WHERE TABLE_NAME  = 'address'").at(0);
            database.write(db, "insert into address (                                       \
                                    address,                                                \
                                    type_id,                                                \
                                    create_by,                                              \
                                    create_date,                                            \
                                    update_by,                                              \
                                    update_date                                             \
                                ) values (                                                  \
                                    '" + ui->te_sertifikat_homeAddress->toPlainText() + "', \
                                    '1',                                                    \
                                    '" + QString::number(id_user) + "',                     \
                                    '" + update_date + "',                                  \
                                    '" + QString::number(id_user) + "',                     \
                                    '" + update_date + "'                                   \
                                );"
            );
            QString new_home_phone_id = database.read(db, "SELECT AUTO_INCREMENT            \
                                                           FROM  INFORMATION_SCHEMA.TABLES  \
                                                           WHERE TABLE_NAME  = 'phone'").at(0);
            database.write(db, "insert into phone (                                 \
                                    number,                                         \
                                    type_id,                                        \
                                    create_by,                                      \
                                    create_date,                                    \
                                    update_by,                                      \
                                    update_date                                     \
                                ) values (                                          \
                                    '" + ui->le_sertifikat_homePhone->text() + "',  \
                                    '1',                                            \
                                    '" + QString::number(id_user) + "',             \
                                    '" + update_date + "',                          \
                                    '" + QString::number(id_user) + "',             \
                                    '" + update_date + "'                           \
                                );"
            );
            QString new_office_address_id = database.read(db, "SELECT AUTO_INCREMENT            \
                                                               FROM  INFORMATION_SCHEMA.TABLES  \
                                                               WHERE TABLE_NAME  = 'address'").at(0);
            database.write(db, "insert into address (                                           \
                                    address,                                                    \
                                    type_id,                                                    \
                                    create_by,                                                  \
                                    create_date,                                                \
                                    update_by,                                                  \
                                    update_date                                                 \
                                ) values (                                                      \
                                    '" + ui->te_sertifikat_officeAddress->toPlainText() + "',   \
                                    '2',                                                        \
                                    '" + QString::number(id_user) + "',                         \
                                    '" + update_date + "',                                      \
                                    '" + QString::number(id_user) + "',                         \
                                    '" + update_date + "'                                       \
                                );"
            );
            QString new_office_phone_id = database.read(db, "SELECT AUTO_INCREMENT              \
                                                             FROM  INFORMATION_SCHEMA.TABLES    \
                                                             WHERE TABLE_NAME  = 'phone'").at(0);
            database.write(db, "insert into phone (                                     \
                                    number,                                             \
                                    type_id,                                            \
                                    create_by,                                          \
                                    create_date,                                        \
                                    update_by,                                          \
                                    update_date                                         \
                                ) values (                                              \
                                    '" + ui->le_sertifikat_officePhone->text() + "',    \
                                    '2',                                                \
                                    '" + QString::number(id_user) + "',                 \
                                    '" + update_date + "',                              \
                                    '" + QString::number(id_user) + "',                 \
                                    '" + update_date + "'                               \
                                );"
            );
            QString new_email_address_id = database.read(db, "SELECT AUTO_INCREMENT              \
                                                              FROM  INFORMATION_SCHEMA.TABLES    \
                                                              WHERE TABLE_NAME  = 'address'").at(0);
            database.write(db, "insert into address (                                       \
                                    address,                                                \
                                    type_id,                                                \
                                    create_by,                                              \
                                    create_date,                                            \
                                    update_by,                                              \
                                    update_date                                             \
                                ) values (                                                  \
                                    '" + ui->le_sertifikat_emailAddress->text() + "',       \
                                    '3',                                                    \
                                    '" + QString::number(id_user) + "',                     \
                                    '" + update_date + "',                                  \
                                    '" + QString::number(id_user) + "',                     \
                                    '" + update_date + "'                                   \
                                );"
            );
            QString new_ident_number_id = database.read(db, "SELECT AUTO_INCREMENT              \
                                                             FROM  INFORMATION_SCHEMA.TABLES    \
                                                             WHERE TABLE_NAME  = 'number'").at(0);
            database.write(db, "insert into number (                                    \
                                    number,                                             \
                                    type_id,                                            \
                                    create_by,                                          \
                                    create_date,                                        \
                                    update_by,                                          \
                                    update_date                                         \
                                ) values (                                              \
                                    '" + ui->le_sertifikat_identNumber->text() + "',    \
                                    '1',                                                \
                                    '" + QString::number(id_user) + "',                 \
                                    '" + update_date + "',                              \
                                    '" + QString::number(id_user) + "',                 \
                                    '" + update_date + "'                               \
                                );"
            );
            QString new_identitifacion_id = database.read(db, "SELECT AUTO_INCREMENT            \
                                                               FROM  INFORMATION_SCHEMA.TABLES  \
                                                               WHERE TABLE_NAME  = 'identification'").at(0);
            database.write(db, "insert into identification (                                                \
                                    name,                                                                   \
                                    ident_number_id,                                                        \
                                    birth_place,                                                            \
                                    birth_date,                                                             \
                                    gender,                                                                 \
                                    home_address_id,                                                        \
                                    home_phone_id,                                                          \
                                    office_address_id,                                                      \
                                    office_phone_id,                                                        \
                                    email_address_id,                                                       \
                                    create_by,                                                              \
                                    create_date,                                                            \
                                    update_by,                                                              \
                                    update_date                                                             \
                                ) values (                                                                  \
                                    '" + ui->le_sertifikat_fullName->text() + "',                           \
                                    '" + new_ident_number_id + "',                                          \
                                    '" + ui->le_sertifikat_birthPlace->text() + "',                         \
                                    '" + ui->de_sertifikat_birthDate->date().toString("dd-MM-yyyy") + "',   \
                                    '" + ui->cb_sertifikat_gender->currentText() + "',                      \
                                    '" + new_home_address_id + "',                                          \
                                    '" + new_home_phone_id + "',                                            \
                                    '" + new_office_address_id + "',                                        \
                                    '" + new_office_phone_id + "',                                          \
                                    '" + new_email_address_id + "',                                         \
                                    '" + QString::number(id_user) + "',                                     \
                                    '" + update_date + "',                                                  \
                                    '" + QString::number(id_user) + "',                                     \
                                    '" + update_date + "'                                                   \
                                );"
            );
            QString new_photo_picture_id = database.read(db, "SELECT AUTO_INCREMENT              \
                                                              FROM  INFORMATION_SCHEMA.TABLES    \
                                                              WHERE TABLE_NAME  = 'picture'").at(0);
            database.write(db, "insert into picture (                                   \
                                    path,                                               \
                                    type_id,                                            \
                                    create_by,                                          \
                                    create_date,                                        \
                                    update_by,                                          \
                                    update_date                                         \
                                ) values (                                              \
                                    '" + QString(PATH_FILE) + "/" + newPhotoName + "',  \
                                    '1',                                                \
                                    '" + QString::number(id_user) + "',                 \
                                    '" + update_date + "',                              \
                                    '" + QString::number(id_user) + "',                 \
                                    '" + update_date + "'                               \
                                );"
            );
            QString new_certification_picture_id = database.read(db, "SELECT AUTO_INCREMENT              \
                                                                      FROM  INFORMATION_SCHEMA.TABLES    \
                                                                      WHERE TABLE_NAME  = 'picture'").at(0);
            database.write(db, "insert into picture (                                           \
                                    path,                                                       \
                                    type_id,                                                    \
                                    create_by,                                                  \
                                    create_date,                                                \
                                    update_by,                                                  \
                                    update_date                                                 \
                                ) values (                                                      \
                                    '" + QString(PATH_FILE) + "/" + newCertificateName + "',    \
                                    '2',                                                        \
                                    '" + QString::number(id_user) + "',                         \
                                    '" + update_date + "',                                      \
                                    '" + QString::number(id_user) + "',                         \
                                    '" + update_date + "'                                       \
                                );"
            );
            QString new_registration_number_id = database.read(db, "SELECT AUTO_INCREMENT              \
                                                                    FROM  INFORMATION_SCHEMA.TABLES    \
                                                                    WHERE TABLE_NAME  = 'number'").at(0);
            database.write(db, "insert into number (                                        \
                                    number,                                                 \
                                    type_id,                                                \
                                    create_by,                                              \
                                    create_date,                                            \
                                    update_by,                                              \
                                    update_date                                             \
                                ) values (                                                  \
                                    '" + ui->le_sertifikat_registrationNumber->text() + "', \
                                    '2',                                                    \
                                    '" + QString::number(id_user) + "',                     \
                                    '" + update_date + "',                                  \
                                    '" + QString::number(id_user) + "',                     \
                                    '" + update_date + "'                                   \
                                );"
            );
            QString new_cert_number_id = database.read(db, "SELECT AUTO_INCREMENT              \
                                                            FROM  INFORMATION_SCHEMA.TABLES    \
                                                            WHERE TABLE_NAME  = 'number'").at(0);
            database.write(db, "insert into number (                                    \
                                    number,                                             \
                                    type_id,                                            \
                                    create_by,                                          \
                                    create_date,                                        \
                                    update_by,                                          \
                                    update_date                                         \
                                ) values (                                              \
                                    '" + ui->le_sertifikat_certNumber->text() + "',     \
                                    '3',                                                \
                                    '" + QString::number(id_user) + "',                 \
                                    '" + update_date + "',                              \
                                    '" + QString::number(id_user) + "',                 \
                                    '" + update_date + "'                               \
                                );"
            );
            QString new_kbji_number_id = database.read(db, "SELECT AUTO_INCREMENT              \
                                                            FROM  INFORMATION_SCHEMA.TABLES    \
                                                            WHERE TABLE_NAME  = 'number'").at(0);
            database.write(db, "insert into number (                                    \
                                    number,                                             \
                                    type_id,                                            \
                                    create_by,                                          \
                                    create_date,                                        \
                                    update_by,                                          \
                                    update_date                                         \
                                ) values (                                              \
                                    '" + ui->le_sertifikat_kbjiNumber->text() + "',     \
                                    '4',                                                \
                                    '" + QString::number(id_user) + "',                 \
                                    '" + update_date + "',                              \
                                    '" + QString::number(id_user) + "',                 \
                                    '" + update_date + "'                               \
                                );"
            );
            QString new_rfid_number_id = database.read(db, "SELECT AUTO_INCREMENT              \
                                                            FROM  INFORMATION_SCHEMA.TABLES    \
                                                            WHERE TABLE_NAME  = 'rfid'").at(0);
            database.write(db, "insert into rfid (                              \
                                    number,                                     \
                                    create_by,                                  \
                                    create_date,                                \
                                    update_by,                                  \
                                    update_date                                 \
                                ) values (                                      \
                                    '" + ui->le_sertifikat_rfid->text() + "',   \
                                    '" + QString::number(id_user) + "',         \
                                    '" + update_date + "',                      \
                                    '" + QString::number(id_user) + "',         \
                                    '" + update_date + "'                       \
                                );"
            );
            QString new_assessment_place_id = database.read(db, "SELECT AUTO_INCREMENT              \
                                                                 FROM  INFORMATION_SCHEMA.TABLES    \
                                                                 WHERE TABLE_NAME  = 'address'").at(0);
            database.write(db, "insert into address (                                           \
                                    address,                                                    \
                                    type_id,                                                    \
                                    create_by,                                                  \
                                    create_date,                                                \
                                    update_by,                                                  \
                                    update_date                                                 \
                                ) values (                                                      \
                                    '" + ui->te_sertifikat_assessmentPlace->toPlainText() + "', \
                                    '4',                                                        \
                                    '" + QString::number(id_user) + "',                         \
                                    '" + update_date + "',                                      \
                                    '" + QString::number(id_user) + "',                         \
                                    '" + update_date + "'                                       \
                                );"
            );
            database.write(db, "insert into certification (                                                     \
                                    rfid,                                                                       \
                                    photo_picture_id,                                                           \
                                    cert_number_id,                                                             \
                                    certification_picture_id,                                                   \
                                    reg_number_id,                                                              \
                                    kbji_number_id,                                                             \
                                    competence,                                                                 \
                                    identification_id,                                                          \
                                    start_date,                                                                 \
                                    end_date,                                                                   \
                                    assessor_id,                                                                \
                                    assessment_place_id,                                                        \
                                    assessment_date,                                                            \
                                    create_by,                                                                  \
                                    create_date,                                                                \
                                    update_by,                                                                  \
                                    update_date                                                                 \
                                ) values (                                                                      \
                                    '" + new_rfid_number_id + "',                                               \
                                    '" + new_photo_picture_id + "',                                             \
                                    '" + new_cert_number_id + "',                                               \
                                    '" + new_certification_picture_id + "',                                     \
                                    '" + new_registration_number_id + "',                                       \
                                    '" + new_kbji_number_id + "',                                               \
                                    '" + ui->le_sertifikat_competence->text() + "',                             \
                                    '" + new_identitifacion_id + "',                                            \
                                    '" + ui->de_sertifikat_startDate->date().toString("dd-MM-yyyy") + "',       \
                                    '" + ui->de_sertifikat_endDate->date().toString("dd-MM-yyyy")+ "',          \
                                    '" + assessor.at(ui->cb_sertifikat_assessor->currentIndex()*2) + "',        \
                                    '" + new_assessment_place_id + "',                                          \
                                    '" + ui->de_sertifikat_assessmentDate->date().toString("dd-MM-yyyy")+ "',   \
                                    '" + QString::number(id_user) + "',                                         \
                                    '" + update_date + "',                                                      \
                                    '" + QString::number(id_user) + "',                                         \
                                    '" + update_date + "'                                                       \
                                );"
            );
        }
        this->close();
    } else {
        QMessageBox::warning(this, "Form - LSPE", "Please complete the form ..");
    }
}

void Form_Pendaftaran::on_pb_sertifikat_cancel_clicked()
{
    this->close();
}

void Form_Pendaftaran::on_pb_sertifikat_findPhoto_clicked()
{
    QString photo =
        QFileDialog::getOpenFileName(this, "Find a Photo", QString(),
            "Images (*.png *.jpeg *.jpg)");
//    QString photo =
//        QFileDialog::getOpenFileName(this, "Open File", "directoryToOpen",
//            "Images (*.png *.jpeg *.jpg);;Text files (*.txt);;XML files (*.xml)");
    ui->le_sertifikat_photoFile->setText(photo);
}

void Form_Pendaftaran::on_pb_sertifikat_findCert_clicked()
{
    QString certification =
        QFileDialog::getOpenFileName(this, "Find a Certification", QString(),
            "File (*.pdf)");
    ui->le_sertifikat_certFile->setText(certification);
}

void Form_Pendaftaran::save_file(QString path, QString newName)
{
    QFile folderTarget(PATH_FILE);
    if (!folderTarget.exists()) {
        QDir dir;
        dir.mkpath(PATH_FILE);
    }
    QFile::copy(path, QString(PATH_FILE)+"/"+newName);
    //    QFile::rename(QString(PATH_FILE) + "/" + oldName, QString(PATH_FILE) + "/" + newName);
}

bool Form_Pendaftaran::validationAdminForm()
{
    if (
            ui->le_admin_username->text().isEmpty() ||
            ui->le_admin_fullName->text().isEmpty() ||
            ui->le_admin_identNumber->text().isEmpty() ||
            ui->le_admin_birthPlace->text().isEmpty() ||
            ui->te_admin_homeAddress->toPlainText().isEmpty() ||
            ui->le_admin_homePhone->text().isEmpty() ||
            ui->le_admin_emailAddress->text().isEmpty()
            ) {
        return false;
    } else {
        return true;
    }
}

bool Form_Pendaftaran::validationassessorForm()
{
    if (
            ui->le_assessor_fullName->text().isEmpty() ||
            ui->le_assessor_identNumber->text().isEmpty() ||
            ui->le_assessor_birthPlace->text().isEmpty() ||
            ui->te_assessor_homeAddress->toPlainText().isEmpty() ||
            ui->le_assessor_homePhone->text().isEmpty() ||
            ui->le_assessor_emailAddress->text().isEmpty()
            ) {
        return false;
    } else {
        return true;
    }
}

bool Form_Pendaftaran::validationSertifikatForm()
{
//    bool competenceValid = false;
//    for (int i = 0; i < competence.length(); i+=2) {
//        if (competence.at(i+1) == ui->cb_sertifikat_competence->currentText()) {
//            ui->cb_sertifikat_competence->setCurrentIndex(i);
//            competenceValid = true;
//        }
//    }
    bool assessorValid = false;
    for (int i = 0; i < assessor.length(); i+=2) {
        if (assessor.at(i+1) == ui->cb_sertifikat_assessor->currentText()) {
            ui->cb_sertifikat_assessor->setCurrentIndex(i);
            assessorValid = true;
        }
    }
    if (
            ui->le_sertifikat_fullName->text().isEmpty() ||
            ui->le_sertifikat_photoFile->text().isEmpty() ||
            ui->le_sertifikat_identNumber->text().isEmpty() ||
            ui->te_sertifikat_homeAddress->toPlainText().isEmpty() ||
            ui->le_sertifikat_homePhone->text().isEmpty() ||
            ui->te_sertifikat_officeAddress->toPlainText().isEmpty() ||
            ui->le_sertifikat_officePhone->text().isEmpty() ||
            ui->le_sertifikat_rfid->text().isEmpty() ||
            ui->le_sertifikat_certNumber->text().isEmpty() ||
            ui->le_sertifikat_certFile->text().isEmpty() ||
            ui->le_sertifikat_registrationNumber->text().isEmpty() ||
            ui->le_sertifikat_kbjiNumber->text().isEmpty() ||
            (
                QDateTime::fromString(ui->de_sertifikat_startDate->date().toString("ddMMyyyy"), "ddMMyyyy").toTime_t() >
                QDateTime::fromString(ui->de_sertifikat_endDate->date().toString("ddMMyyyy"), "ddMMyyyy").toTime_t()
                ) ||
            (
                QDateTime::fromString(ui->de_sertifikat_endDate->date().toString("ddMMyyyy"), "ddMMyyyy").toTime_t() <=
                QDateTime::currentDateTime().toTime_t()
                ) ||
            !assessorValid
            ) {
        return false;
    } else {
        return true;
    }
}
