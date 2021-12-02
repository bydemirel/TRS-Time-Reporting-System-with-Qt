include "timespentproject.h"
#include "ui_timespentproject.h"

timeSpentProject::timeSpentProject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::timeSpentProject)
{
    ui->setupUi(this);
}

timeSpentProject::~timeSpentProject()
{
    delete ui;
}
