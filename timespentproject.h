#ifndef TIMESPENTPROJECT_H
#define TIMESPENTPROJECT_H

#include <QDialog>

namespace Ui {
class timeSpentProject;
}

class timeSpentProject : public QDialog
{
    Q_OBJECT

public:
    explicit timeSpentProject(QWidget *parent = nullptr);
    ~timeSpentProject();

private:
    Ui::timeSpentProject *ui;
};

#endif // TIMESPENTPROJECT_H
