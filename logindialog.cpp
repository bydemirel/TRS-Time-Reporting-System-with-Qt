#include "logindialog.h"
#include "mainwindow.h"
#include "ui_logindialog.h"

QString managerName; // Global variable

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    setWindowTitle("Login Screen");


}

LoginDialog::~LoginDialog()
{
    delete ui;
}

QString LoginDialog::getManagerName()
{
    if(managerName.isEmpty()) // If user didn't enter a name program will close.
    {
        QApplication::quit();
    }
    return managerName;
}

void LoginDialog::on_buttonBox_accepted()
{
    if(ui->managerNameEdit->text().isEmpty()) // If user didn't enter a name program will close.
    {
        QApplication::quit();
    }
    else
    {
        managerName = ui->managerNameEdit->text();

        qWarning() << "Manager Name: "<<managerName;
    }

}

