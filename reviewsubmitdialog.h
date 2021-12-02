#ifndef REVIEWSUBMITDIALOG_H
#define REVIEWSUBMITDIALOG_H

#include <QDialog>

namespace Ui {
class reviewSubmitDialog;
}

class reviewSubmitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit reviewSubmitDialog(QWidget *parent = nullptr);
    ~reviewSubmitDialog();

private slots:
    void on_buttonBox_accepted();

    bool checkTwiceEntry();

private:
    Ui::reviewSubmitDialog *ui;
};

#endif // REVIEWSUBMITDIALOG_H
