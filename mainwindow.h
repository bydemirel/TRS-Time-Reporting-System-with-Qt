#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "activitydialog.h"
#include "menudialog.h"
#include "logindialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString getCalendarValue();

    QStringList getActivityList();

    void setActivityList(QStringList);

private slots:
    void on_createNewActivityButton_clicked();

    void on_addNewButton_clicked();

    void setUIVisibilityBeforeLogin();

    void setUIVisibilityAfterLogin();

    void decideManagerOrUser();

    void on_refreshButton_clicked();

    void on_monthlyReport_clicked();

    void on_loginButton_clicked();

    void on_calendarWidget_selectionChanged();

    void on_tabWidget_tabBarClicked(int index);

    void on_changeUser_clicked();

    void on_submitButton_clicked();

    void on_inactivateButton_clicked();

    void on_activateButton_clicked();

    void on_selectDayButton_clicked();

    void on_reviewButton_clicked();

private:
    Ui::MainWindow *ui;


};
#endif // MAINWINDOW_H
