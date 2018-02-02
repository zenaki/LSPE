#include "form_peserta.h"
#include "ui_form_peserta.h"

Form_Peserta::Form_Peserta(QWidget *parent, QSqlDatabase db_parent, int id, int id_user) :
    QDialog(parent),
    ui(new Ui::Form_Peserta)
{
    ui->setupUi(this);
    this->setWindowTitle("Detail Sertifikat");

    pdf = new QAxWidget(this);
    if(!pdf->setControl("Adobe PDF Reader"))
        QMessageBox::critical(this, "Error", "Make sure you have Adobe Reader (and its ActiveX) installed!");

    ui->splitter->addWidget(pdf);
    ui->splitter->widget(0)->setMaximumWidth(500);

    db = db_parent;
    target_id = id;
    user_id = id_user;

    setWindow();
}

Form_Peserta::~Form_Peserta()
{
    delete ui;
}

void Form_Peserta::setWindow()
{
    QStringList result = database.read(db,
                                  "select                                                                                           \
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
                                       (select path from picture where id = c.photo_picture_id) as photo_picture,                   \
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
                                       id = " + QString::number(target_id)
            );
    ui->lbl_fullName->setText(result.at(0));
    ui->lbl_identNumber->setText(result.at(1));
    ui->lbl_birthPlace->setText(result.at(2));
    ui->lbl_birthDate->setText(QDate::fromString(result.at(3), "dd-MM-yyyy").toString("dd MMMM yyyy"));
    ui->lbl_gender->setText(result.at(4));
    ui->lbl_addressHome->setText(result.at(5));
    ui->lbl_addressHome->setWordWrap(true);
    ui->lbl_phoneHome->setText(result.at(6));
    ui->lbl_addressOffice->setText(result.at(7));
    ui->lbl_addressOffice->setWordWrap(true);
    ui->lbl_phoneOffice->setText(result.at(8));
    ui->lbl_emailAddress->setText(result.at(9));
    ui->photo->setViewMode(QListWidget::IconMode);
    ui->photo->setIconSize(QSize(300,400));
    ui->photo->setResizeMode(QListWidget::Adjust);
    ui->photo->addItem(new QListWidgetItem(QIcon(result.at(10)), result.at(0)));
//    qDebug() << result.at(10);
    ui->lbl_certificationNumber->setText(result.at(11));
    pdf->dynamicCall("LoadFile(QString)",result.at(12));
//    qDebug() << result.at(12);

//    pdf = new QAxWidget(this);
//    if(!pdf->setControl("Adobe PDF Reader")) {
//        QMessageBox::critical(this, "Error", "Make sure you have Adobe Reader (and its ActiveX) installed!");
//    } else {
//        pdf->dynamicCall("LoadFile(QString)",result.at(12));
//    }

//    QString pdf = QUrl::toPercentEncoding("file:///"+QDir::currentPath()+"/"+result.at(12));
//    QString pdf = "file:///"+QDir::currentPath()+"/"+result.at(12);
//    QString pdf = "file:///"+QDir::currentPath()+"/File/test.html";
//    QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::currentPath()+"/"+result.at(12)));
//    webView->setContent(pdf.toUtf8().data(), "application/pdf");
//    webView->setHtml(pdf); /*webView->show();*/

//        webView->setRenderHint(QPainter::Antialiasing);
//        webView->setRenderHint(QPainter::TextAntialiasing);
//        weLbView->setRenderHint(QPainter::SmoothPixmapTransform);
//        webView->setRenderHint(QPainter::HighQualityAntialiasing);

//        webView->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
//        webView->settings()->setAttribute(QWebSettings::AcceleratedCompositingEnabled, true);
//        webView->settings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, true);
//        webView->settings()->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, true);
//        webView->settings()->setAttribute(QWebSettings::LocalStorageEnabled, true); // Note: Added as well to allow PDF history to be kept.
//        webView->settings()->setAttribute(QWebSettings::JavascriptEnabled, true);

//        webView->page()->mainFrame()->load(QUrl(pdf));

    ui->lbl_registrationNumber->setText(result.at(13));
    ui->lbl_kbjiNumber->setText(result.at(14));
    ui->lbl_competence->setText(result.at(15));
    ui->lbl_startDate->setText(QDate::fromString(result.at(16), "dd-MM-yyyy").toString("dd MMMM yyyy"));
    ui->lbl_endDate->setText(QDate::fromString(result.at(17), "dd-MM-yyyy").toString("dd MMMM yyyy"));
    ui->lbl_assessor->setText(result.at(18));
    ui->lbl_assessmentPlace->setText(result.at(19));
    ui->lbl_assessmentDate->setText(QDate::fromString(result.at(20), "dd-MM-yyyy").toString("dd MMMM yyyy"));
}
void Form_Peserta::on_pb_edit_clicked()
{
    Form_Pendaftaran *edit_peserta = new Form_Pendaftaran(this, db, 3, user_id, true, target_id);
    edit_peserta->exec();

    edit_peserta->deleteLater();
    setWindow();
}

void Form_Peserta::on_pb_delete_clicked()
{
    database.write(db, "delete from number where id = (                         \
                            select ident_number_id                              \
                            from identification where id = (                    \
                                select identification_id                        \
                                from certification                              \
                                where id = " + QString::number(target_id) + "   \
                            )                                                   \
                        )"
            );
    database.write(db, "delete from address where id = (                        \
                            select home_address_id                              \
                            from identification where id = (                    \
                                select identification_id                        \
                                from certification                              \
                                where id = " + QString::number(target_id) + "   \
                            )                                                   \
                        )"
            );
    database.write(db, "delete from phone where id = (                          \
                            select home_phone_id                                \
                            from identification where id = (                    \
                                select identification_id                        \
                                from certification                              \
                                where id = " + QString::number(target_id) + "   \
                            )                                                   \
                        )"
            );
    database.write(db, "delete from address where id = (                        \
                            select office_address_id                            \
                            from identification where id = (                    \
                                select identification_id                        \
                                from certification                              \
                                where id = " + QString::number(target_id) + "   \
                            )                                                   \
                        )"
            );
    database.write(db, "delete from phone where id = (                          \
                            select office_phone_id                              \
                            from identification where id = (                    \
                                select identification_id                        \
                                from certification                              \
                                where id = " + QString::number(target_id) + "   \
                            )                                                   \
                        )"
            );
    database.write(db, "delete from address where id = (                        \
                            select email_address_id                             \
                            from identification where id = (                    \
                                select identification_id                        \
                                from certification                              \
                                where id = " + QString::number(target_id) + "   \
                            )                                                   \
                        )"
            );
    database.write(db, "delete from identification where id = (                 \
                            select identification_id                            \
                            from certification                                  \
                            where id = " + QString::number(target_id) + "       \
                        )"
            );
    database.write(db, "delete from picture where id = (                        \
                            select photo_picture_id                             \
                            from certification                                  \
                            where id = " + QString::number(target_id) + "       \
                        )"
            );
    database.write(db, "delete from number where id = (                         \
                            select cert_number_id                               \
                            from certification                                  \
                            where id = " + QString::number(target_id) + "       \
                        )"
            );
    database.write(db, "delete from picture where id = (                        \
                            select certification_picture_id                     \
                            from certification                                  \
                            where id = " + QString::number(target_id) + "       \
                        )"
            );
    database.write(db, "delete from number where id = (                         \
                            select reg_number_id                                \
                            from certification                                  \
                            where id = " + QString::number(target_id) + "       \
                        )"
            );
    database.write(db, "delete from number where id = (                         \
                            select kbji_number_id                               \
                            from certification                                  \
                            where id = " + QString::number(target_id) + "       \
                        )"
            );
    database.write(db, "delete from certification where id = " + QString::number(target_id));
    this->close();
}
