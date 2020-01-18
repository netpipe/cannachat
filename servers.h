#ifndef SERVERS_H
#define SERVERS_H

#include <QDialog>

namespace Ui {
class ServerDlg;
}

class ServerDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ServerDlg(QWidget *parent = 0);
    ~ServerDlg();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_cancel_clicked();

private:
    Ui::ServerDlg *ui;
};

#endif // SERVERS_H
