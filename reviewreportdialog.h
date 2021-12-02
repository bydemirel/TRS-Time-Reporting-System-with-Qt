#ifndef REVIEWREPORTDIALOG_H
#define REVIEWREPORTDIALOG_H

#include <QDialog>

namespace Ui {
class ReviewReportDialog;
}

class ReviewReportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReviewReportDialog(QWidget *parent = nullptr);
    ~ReviewReportDialog();

private slots:

private:
    Ui::ReviewReportDialog *ui;
};

#endif // REVIEWREPORTDIALOG_H
