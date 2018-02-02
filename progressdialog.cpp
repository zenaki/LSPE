#include "progressdialog.h"
#include "ui_progressdialog.h"

ProgressDialog::ProgressDialog(QWidget *parent,
                               QSqlDatabase db_parent,
                               int user_id) :
    QDialog(parent),
    ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);

    db = db_parent;
    id_user = user_id;
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}

void ProgressDialog::Export(QString file_path)
{
    progressVal = 0;
    ui->progressBar->reset();
    ui->Description->clear();

    assessor = get_assessor();
    certification = get_certification();

    ui->Description->append(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + "::START Export");
    ui->progressBar->setRange(0,(assessor.length()/10)+(certification.length()/22));

    save_file(file_path);

    QMessageBox::information(this, "Export File", "Exporting File Success ...");
}

void ProgressDialog::Import(QString file_path)
{
    progressVal = 0;
    ui->progressBar->reset();
    ui->Description->clear();

    QStringList data = get_data(file_path);
    if (progressVal == 0) return;

//    ui->progressBar->setRange(0,progressVal);
//    progressVal = 0;

    set_data(data);

//    this->close();
    QMessageBox::information(this, "Import File", "Importing File Success ...");
}

QStringList ProgressDialog::get_data(QString path)
{
    // Read CSVc
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return QStringList();
    }

    QStringList data;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        data.append(line.replace("'", "").replace("\"", ""));
        progressVal++;
//        data.append(line.split(',').first());
    }
    return data;
    // progressVal++;
}

void ProgressDialog::set_data(QStringList data)
{
    // Write Data to Database
    for (int i = 0; i < data.length(); i++) {
        if (data.at(i).split(",").at(0) == "[assessor]") {
            QStringList list_temp = data.at(i).split(",");
            int assessor_length = 0;
            for (int j = 0; j < list_temp.length(); j++) {
                list_temp[j].replace("\n", "");
                if (list_temp.at(j) != "") assessor_length++;
            }
            i++;
            while (i <= data.length()-1 && QString(" "+data.at(i).split(",").at(0)).indexOf("#") > 0) {
                for (int j = 0; j < assessor_length; j++) {
                    assessor.append(data.at(i).split(",").at(j));
                }
                i++;
            }
        }
        if (data.at(i).split(",").at(0) == "[certification]") {
            QStringList list_temp = data.at(i).split(",");
            int certification_length = 0;
            for (int j = 0; j < list_temp.length(); j++) {
                list_temp[j].replace("\n", "");
                if (list_temp.at(j) != "") certification_length++;
            }
            i++;
            while (i <= data.length()-1 && QString(" "+data.at(i).split(",").at(0)).indexOf("#") > 0) {
                for (int j = 0; j < certification_length; j++) {
                    certification.append(data.at(i).split(",").at(j));
                }
                i++;
            }
        }
    }
    ui->Description->append(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + "::START Import");
    ui->progressBar->setRange(0,(assessor.length()/11)+(certification.length()/23));
    progressVal = 0;
    for (int i = 0; i < assessor.length(); i+=11) {
        ui->Description->append(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") +
                                "::Insert Assessor " + assessor.at(i+1));
        insertAssessor(
//                    assessor.at(i),     /*  [assessor]              */
                    assessor.at(i+1),   /*  [full_name]             */
                    assessor.at(i+2),   /*  [identification_number] */
                    assessor.at(i+3),   /*  [birth_place]           */
                    assessor.at(i+4),   /*  [birth_date]            */
                    assessor.at(i+5),   /*  [gender]                */
                    assessor.at(i+6),   /*  [home_address]          */
                    assessor.at(i+7),   /*  [home_phone]            */
                    assessor.at(i+8),   /*  [office_address]        */
                    assessor.at(i+9),   /*  [office_phone]          */
                    assessor.at(i+10)   /*  [email_address]         */
                    );
         ui->progressBar->setValue(progressVal++);
    }
    for (int i = 0; i < certification.length(); i+=23) {
        ui->Description->append(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") +
                                "::Insert Certification " + certification.at(i+8));
        insertCertification(
//                    certification.at(i),    /*  [certification]         */
                    certification.at(i+1),  /*  [rfid]                  */
                    certification.at(i+2),  /*  [photo_path]            */
                    certification.at(i+3),  /*  [certification_number]  */
                    certification.at(i+4),  /*  [certification_path]    */
                    certification.at(i+5),  /*  [registration_number]   */
                    certification.at(i+6),  /*  [blanko_number]         */
                    certification.at(i+7),  /*  [competence]            */
                    certification.at(i+8),  /*  [full_name]             */
                    certification.at(i+9),  /*  [identification_number] */
                    certification.at(i+10), /*  [birth_place]           */
                    certification.at(i+11), /*  [birth_date]            */
                    certification.at(i+12), /*  [gender]                */
                    certification.at(i+13), /*  [home_address]          */
                    certification.at(i+14), /*  [home_phone]            */
                    certification.at(i+15), /*  [office_address]        */
                    certification.at(i+16), /*  [office_phone]          */
                    certification.at(i+17), /*  [email_address]         */
                    certification.at(i+18), /*  [start_date]            */
                    certification.at(i+19), /*  [end_date]              */
                    certification.at(i+20), /*  [assessor_id]           */
                    certification.at(i+21), /*  [assessment_place]      */
                    certification.at(i+22)  /*  [assessment_date]       */
                    );
        ui->progressBar->setValue(progressVal++);
    }
    ui->Description->append(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + "::END Import");
}

void ProgressDialog::on_pbDetail_clicked()
{
    if (ui->Description->isHidden()) {
        ui->pbDetail->setText("Hide Detail");
        ui->Description->setHidden(false);
    } else {
        ui->pbDetail->setText("Show Detail");
        ui->Description->setHidden(true);
    }
}

void ProgressDialog::on_pbCancel_clicked()
{
    cancel = true;
}

void ProgressDialog::insertAssessor(
            QString fullName,
            QString identNumber,
            QString birthPlace,
            QString birthDate,
            QString gender,
            QString homeAddress,
            QString homePhone,
            QString officeAddress,
            QString officePhone,
            QString emailAddress
        )
{
    QString update_date = QString::number(QDateTime::currentDateTime().toTime_t());
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
                            '" + homeAddress + "',   \
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
                            '" + homePhone + "',    \
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
                            '" + officeAddress + "', \
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
                            '" + officePhone + "',  \
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
                            '" + emailAddress + "', \
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
                            '" + identNumber + "',  \
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
                            '" + fullName + "',                          \
                            '" + new_ident_number_id + "',                                      \
                            '" + birthPlace + "',                        \
                            '" + birthDate + "',  \
                            '" + gender + "',                     \
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

void ProgressDialog::insertCertification(
        QString rfid,
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
        QString assessmentDate
        )
{
    QString update_date = QString::number(QDateTime::currentDateTime().toTime_t());
    QString newPhotoName = "photo_" +
            identNumber + "_" +
            fullName + "_" +
            certNumber;
    this->save_file(photoFile, newPhotoName);
    QString newCertificateName = "certificate_" +
            identNumber + "_" +
            fullName + "_" +
            certNumber;
    this->save_file(certFile, newCertificateName);

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
                            '" + homeAddress + "', \
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
                            '" + homePhone + "',  \
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
                            '" + officeAddress + "',   \
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
                            '" + officePhone + "',    \
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
                            '" + emailAddress + "',       \
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
                            '" + identNumber + "',    \
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
                            '" + fullName + "',                           \
                            '" + new_ident_number_id + "',                                          \
                            '" + birthPlace + "',                         \
                            '" + birthDate + "',   \
                            '" + gender + "',                      \
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
                            '" + registrationNumber + "', \
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
                            '" + certNumber + "',     \
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
                            '" + kbjiNumber + "',     \
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
                            '" + rfid + "',   \
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
                            '" + assessmentPlace + "', \
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
                            '" + competence + "',                             \
                            '" + new_identitifacion_id + "',                                            \
                            '" + startDate + "',       \
                            '" + endDate + "',          \
                            '" + assessor + "',        \
                            '" + new_assessment_place_id + "',                                          \
                            '" + assessmentDate + "',   \
                            '" + QString::number(id_user) + "',                                         \
                            '" + update_date + "',                                                      \
                            '" + QString::number(id_user) + "',                                         \
                            '" + update_date + "'                                                       \
                        );"
    );
}

void ProgressDialog::save_file(QString path, QString newName)
{
    QFile folderTarget(PATH_FILE);
    if (!folderTarget.exists()) {
        QDir dir;
        dir.mkpath(PATH_FILE);
    }
    QFile::copy(path, QString(PATH_FILE)+"/"+newName);
    //    QFile::rename(QString(PATH_FILE) + "/" + oldName, QString(PATH_FILE) + "/" + newName);
}

QStringList ProgressDialog::get_assessor()
{
    QStringList result = database.read(db,
                                  "select                                                                                   \
                                       i.name,                                                                              \
                                       (select number from number where id = i.ident_number_id) as identification_number,   \
                                       i.birth_place,                                                                       \
                                       i.birth_date,                                                                        \
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
                                       a.identification_id = i.id"
            );
    return result;
}

QStringList ProgressDialog::get_certification()
{
    QStringList result = database.read(db,
                                  "select                                                                                           \
                                       (select number from rfid where id = c.rfid) as rfid,                                         \
                                       (select path from picture where id = c.photo_picture_id) as photo_picture,                   \
                                       (select number from number where id = c.cert_number_id) as certification_number,             \
                                       (select path from picture where id = c.certification_picture_id) as certification_picture,   \
                                       (select number from number where id = c.reg_number_id) as registration_number,               \
                                       (select number from number where id = c.kbji_number_id) as kbji_number,                      \
                                       c.competence,                                                                                \
                                       (select name from identification where id = c.identification_id) as full_name,               \
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
                                       c.start_date,                                                                                \
                                       c.end_date,                                                                                  \
                                       c.assessor_id,                                                                               \
                                       (select address from address where id = c.assessment_place_id) as assessment_place,          \
                                       c.assessment_date                                                                            \
                                   from                                                                                             \
                                       certification c"
            );
    return result;
}

void ProgressDialog::save_file(QString path)
{
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    file.write("[assessor],"
               "[full_name],"
               "[identification_number],"
               "[birth_place],"
               "[birth_date],"
               "[gender],"
               "[home_address],"
               "[home_phone],"
               "[office_address],"
               "[office_phone],"
               "[email_address],"
               ","
               ","
               ","
               ","
               ","
               ","
               ","
               ","
               ","
               ","
               ","
               "\n");
    for (int i = 0; i < assessor.length(); i+=10) {
        QString data = "#" + QString::number((i/10)+1) + "," +
                "'" + assessor.at(i) + "," +
                "'" + assessor.at(i+1) + "," +
                "'" + assessor.at(i+2) + "," +
                "'" + assessor.at(i+3) + "," +
                "'" + assessor.at(i+4) + "," +
                "'" + assessor.at(i+5) + "," +
                "'" + assessor.at(i+6) + "," +
                "'" + assessor.at(i+7) + "," +
                "'" + assessor.at(i+8) + "," +
                "'" + assessor.at(i+9) + "," +
                ","
                ","
                ","
                ","
                ","
                ","
                ","
                ","
                ","
                ","
                ","
                "\n";
        file.write(data.toUtf8().data());
        ui->progressBar->setValue(progressVal++);
    }
    file.write(",,,,,,,,,,,,,,,,,,,,,,\n");
    file.write("[certification],"
               "[rfid],"
               "[photo_path],"
               "[certification_number],"
               "[certification_path],"
               "[registration_number],"
               "[blanko_number],"
               "[competence],"
               "[full_name],"
               "[identification_number],"
               "[birth_place],"
               "[birth_date],"
               "[gender],"
               "[home_address],"
               "[home_phone],"
               "[office_address],"
               "[office_phone],"
               "[email_address],"
               "[start_date],"
               "[end_date],"
               "[assessor],"
               "[assessment_place],"
               "[assessment_date],"
               "\n");
    for (int i = 0; i < certification.length(); i+=22) {
        QString data = "#" + QString::number((i/22)+1) + "," +
                "'" + certification.at(i) + "," +
                "'" + certification.at(i+1) + "," +
                "'" + certification.at(i+2) + "," +
                "'" + certification.at(i+3) + "," +
                "'" + certification.at(i+4) + "," +
                "'" + certification.at(i+5) + "," +
                "'" + certification.at(i+6) + "," +
                "'" + certification.at(i+7) + "," +
                "'" + certification.at(i+8) + "," +
                "'" + certification.at(i+9) + "," +
                "'" + certification.at(i+10) + "," +
                "'" + certification.at(i+11) + "," +
                "'" + certification.at(i+12) + "," +
                "'" + certification.at(i+13) + "," +
                "'" + certification.at(i+14) + "," +
                "'" + certification.at(i+15) + "," +
                "'" + certification.at(i+16) + "," +
                "'" + certification.at(i+17) + "," +
                "'" + certification.at(i+18) + "," +
                "'" + certification.at(i+19) + "," +
                "'" + certification.at(i+20) + "," +
                "'" + certification.at(i+21) + "," +
                "\n";
        file.write(data.toUtf8().data());
        ui->progressBar->setValue(progressVal++);
    }
    file.close();
}
