#ifndef SERVERS_H
#define SERVERS_H

#include <QDialog>

namespace Ui {
class servers;
}

class servers : public QDialog
{
    Q_OBJECT

public:
    explicit servers(QWidget *parent = 0);
    ~servers();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::servers *ui;
};

#endif // SERVERS_H
