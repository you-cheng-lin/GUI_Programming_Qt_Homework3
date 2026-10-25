#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <QStringList>


void Write(QString Filename, QString str)
{
    QFile mFile(Filename);

    if(!mFile.open(QFile::WriteOnly | QFile::Text)){
        qDebug() << "could not open file for write";
        return;
    }

    QTextStream out(&mFile);
    out << str;
    mFile.flush();
    mFile.close();
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    QStringList ColTotle;

    ui->tableWidget->setColumnCount(4);

    ColTotle << QStringLiteral("學號")
             << QStringLiteral("班級")
             << QStringLiteral("姓名")
             << QStringLiteral("電話");

    ui->tableWidget->setHorizontalHeaderLabels(ColTotle);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_2_clicked()
{
    QString filename = QFileDialog::getSaveFileName(
        this,
        QStringLiteral("選擇儲存位置"),
        "",
        QStringLiteral("CSV Files (*.csv);;Text Files (*.txt)")
        );

    if(filename.isEmpty()){
        return;
    }

    QString saveFile = "";

    for(int i = 0; i < ui->tableWidget->rowCount(); i++){
        for(int j = 0; j < ui->tableWidget->columnCount(); j++){

            if(ui->tableWidget->item(i, j) != nullptr){
                saveFile += ui->tableWidget->item(i, j)->text();
            }

            if(j < ui->tableWidget->columnCount() - 1){
                saveFile += ",";
            }
        }

        saveFile += "\n";
    }

    Write(filename, saveFile);
}

void Widget::on_pushButton_clicked()
{
    QTableWidgetItem *inputRow1, *inputRow2, *inputRow3, *inputRow4;

    inputRow1 = new QTableWidgetItem(QString(ui->lineEdit->text()));
    inputRow2 = new QTableWidgetItem(QString(ui->lineEdit_2->text()));
    inputRow3 = new QTableWidgetItem(QString(ui->lineEdit_3->text()));
    inputRow4 = new QTableWidgetItem(QString(ui->lineEdit_4->text()));

    ui->tableWidget->insertRow(ui->tableWidget->rowCount());

    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, inputRow1);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, inputRow2);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 2, inputRow3);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 3, inputRow4);
}


// 匯入 CSV
void Widget::on_pushButton_3_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
        this,
        QStringLiteral("選擇 CSV 檔案"),
        "",
        QStringLiteral("CSV Files (*.csv);;Text Files (*.txt)")
        );

    if(filename.isEmpty()){
        return;
    }

    QFile file(filename);

    if(!file.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "could not open file for read";
        return;
    }

    QTextStream in(&file);

    // 清除原本的表格資料
    ui->tableWidget->setRowCount(0);

    while(!in.atEnd()){
        QString line = in.readLine();

        if(line.isEmpty()){
            continue;
        }

        QStringList data = line.split(",");

        // 確認資料有 4 個欄位
        if(data.size() < 4){
            continue;
        }

        int row = ui->tableWidget->rowCount();

        ui->tableWidget->insertRow(row);

        for(int j = 0; j < 4; j++){
            ui->tableWidget->setItem(
                row,
                j,
                new QTableWidgetItem(data[j])
                );
        }
    }

    file.close();
}