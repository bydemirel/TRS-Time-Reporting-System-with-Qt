#ifndef ACTIVITYDIALOG_H
#define ACTIVITYDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QJsonObject>

namespace Ui {
class ActivityDialog;
}

class ActivityDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ActivityDialog(QWidget *parent = nullptr);
    ~ActivityDialog();
    QList<QString> getProjectCodes();

private slots:
    void on_buttonBox_accepted();

    void updateCategories();

private:
    Ui::ActivityDialog *ui;
};

#endif // ACTIVITYDIALOG_H
