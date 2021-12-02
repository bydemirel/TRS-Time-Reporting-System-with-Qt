#ifndef MENUDIALOG_H
#define MENUDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QJsonObject>

namespace Ui {
class MenuDialog;
}

class MenuDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MenuDialog(QWidget *parent = nullptr);
    ~MenuDialog();

private slots:
    void on_buttonBox_accepted();

    void on_addSubactivityButton_clicked();


private:
    Ui::MenuDialog *ui;
};

#endif // MENUDIALOG_H
