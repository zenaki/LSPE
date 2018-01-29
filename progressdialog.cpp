#include "progressdialog.h"
#include "ui_progressdialog.h"

ProgressDialog::ProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);

    QString path = QFileDialog::getOpenFileName(this, "Open File", QDir::currentPath(), "CSV Files (*.csv)");
    this->Processing(path);
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}

void ProgressDialog::Processing(QString file_path)
{
    progressVal = 0;
    ui->progressBar->reset();
    ui->Description->clear();

    QStringList data = get_data(file_path);
    if (progressVal == 0) return;

    ui->progressBar->setRange(0,progressVal);
    progressVal = 0;

    set_data(data);
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
        data.append(line);
        progressVal++;
//        data.append(line.split(',').first());
    }
    return data;
    // progressVal++;
}

void ProgressDialog::set_data(QStringList data)
{
    // Write Data to Database
    QStringList assessor, certification;
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
    // ui->progressBar->setValue(progressVal++);
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
