#include "reviewreportdialog.h"
#include "ui_reviewreportdialog.h"
#include "activitydialog.h"
#include "mainwindow.h"
#include "logindialog.h"

#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringListModel>
#include <QJsonArray>

ReviewReportDialog::ReviewReportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReviewReportDialog)
{
    ui->setupUi(this);

    setWindowTitle("Monthly Report");


    QStringListModel* model = new QStringListModel(this);

    QStringList list;

    // Read the file and populate the ListView.


        QFile file;
        QString data_json;
        QJsonDocument doc; // We opened a blank page
        QJsonObject rootObj;
        QJsonObject obj; // The innermost objects
        QJsonObject root_obj; // The outermost objects
        QJsonValue val;
        QString code;
        int time;

        QList<QString> projectCodesList = ActivityDialog().getProjectCodes();

        int timeArray[projectCodesList.size()];



        QString codeArray[projectCodesList.size()];

        for (int i = 0; i<projectCodesList.size() ; i++ ) // We reset the array to clear the initial values.
        {
            timeArray[i] = 0;
        }

        for (int i = 0;i<projectCodesList.size() ;i++)
        {
            codeArray[i] = projectCodesList.at(i);
        }

        QString file_path;
        QString current_date = QDate().currentDate().toString("-yyyy-MM");
        file_path = "C:/Qt/Activities/"+LoginDialog().getManagerName()+current_date+".json";


        file.setFileName(file_path);

        if(file.open(QIODevice::ReadOnly | QIODevice:: Text))
        {
                data_json = file.readAll();
                file.close();

                doc = doc.fromJson(data_json.toUtf8());

                rootObj = doc.object();
                QJsonArray rootArray  = rootObj["Entries "].toArray();

                for (auto jsonObj : rootArray)
                {
                    val = jsonObj.toObject().value("code ");
                    code = val.toString();



                    val = jsonObj.toObject().value("time ");
                    time  = val.toInt();


                    for (int i = 0 ; i <  projectCodesList.size() ; i++ )
                    {


                        if(projectCodesList.at(i) == code)
                        {

                            codeArray[i] = code;
                            timeArray[i] += time;

                        }

                    }

                  }

                QString listViewData = "";


                for (int i = 0;i<projectCodesList.size() ; i++ )
                {
                    listViewData += "Project "+codeArray[i]+": " + QString::number(timeArray[i]);
                    list.append(listViewData);

                    listViewData = "";
                }     


                model->setStringList(list);           

                ui->activitiesReviewListView->setModel(model);

           }

}

ReviewReportDialog::~ReviewReportDialog()
{
    delete ui;
}


