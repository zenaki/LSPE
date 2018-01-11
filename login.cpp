#include "login.h"
#include "ui_login.h"

#include "mainwindow.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    this->setWindowTitle("Login - LSPE");

    db = database.connect_db("LSPE_LOGIN");
}

Login::~Login()
{
    delete ui;
}

void Login::on_pb_login_clicked()
{
    bool ok = false;
    if (!ui->le_username->text().isEmpty() && !ui->le_password->text().isEmpty()) {
        QStringList result = database.read(db, "select id, username, password, type_id from user where status = 1");
        for (int i = 0; i < result.length(); i+=4) {
            if (ui->le_username->text() == result.at(i+1)) {
                QString encodedPass = QString(QCryptographicHash::hash(ui->le_password->text().toUtf8(),QCryptographicHash::Md5).toHex());
//                qDebug() << encodedPass;
                if (encodedPass == result.at(i+2)) {
                    ok = true;
                    this->hide();
                    MainWindow *mw = new MainWindow(this, result.at(i).toInt(), result.at(i+3).toInt());
                    mw->showMaximized();
                }
            }
        }
        if (!ok) {
            QMessageBox::warning(this, "Login - LSPE", "Username dan Password masih salah,\n mohon untuk diperiksa kembali ..");
        }
    } else {
        QMessageBox::information(this, "Login - LSPE", "Username dan Password masih kosong,\n mohon untuk dilengkapi ..");
    }
}
