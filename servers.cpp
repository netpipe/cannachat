#include "servers.h"
#include "ui_servers.h"

ServerDlg::ServerDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerDlg)
{
    ui->setupUi(this);
}

ServerDlg::~ServerDlg()
{
    delete ui;
}

void ServerDlg::on_pushButton_2_clicked() //
{

}

void ServerDlg::on_pushButton_3_clicked() //add server
{
//    QString msg = ui->txtMsg->toPlainText();
//    if(msg.isEmpty()){
//        QMessageBox::information(this, "Notice", "Please input message..");
//        return;
//    }

}

void ServerDlg::on_pushButton_4_clicked() //remove server
{
 //   int count = ui->listData->rowCount();
 //   for(int i = 0 ; i < count ; i++)
 //   ui->listData->removeRow(0);
}



void ServerDlg::on_cancel_clicked()
{
   close();
}

void ServerDlg::on_connect_clicked()
{

}
