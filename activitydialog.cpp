#include "activitydialog.h"
#include "ui_activitydialog.h"
#include "mainwindow.h"
#include "logindialog.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QByteArray>
#include <QMessageBox>
#include <QJsonArray>


QList<QString> projectCodes_list;


ActivityDialog::ActivityDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ActivityDialog)
{
    ui->setupUi(this);

    setWindowTitle("Create New/Edit Activity");


    connect(ui->project_combox, SIGNAL(currentTextChanged(QString)),this,SLOT(updateCategories()));



    QFile file;
    QJsonDocument doc;
    QJsonObject obj;
    QString data_json;
    QJsonValue value;
    QJsonValue val;

    file.setFileName("C:/Qt/activity.json");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        data_json = file.readAll();
        file.close();

        doc = doc.fromJson(data_json.toUtf8());

        obj = doc.object();

        qWarning() << obj; // QJsonObject({"Activities ":[{"SubActivities ":[{"code ":"database","code1 ":"other"}],"active ":true,"budget ":125,"code ":"Argus-123","manager ":"baris","name ":"Argus"}]})

        value = obj.value(QString("Activities "));

        qWarning() << "***********";

        qWarning() << value; // QJsonValue(array, QJsonArray([{"SubActivities ":[{"code ":"database","code1 ":"other"}],"active ":true,"budget ":125,"code ":"Argus-123","manager ":"baris","name ":"Argus"}]))

        QJsonArray activitiesArray = value.toArray();

        qWarning() << "***********";

        qWarning() << activitiesArray;  // QJsonArray([{"SubActivities ":[{"code ":"database","code1 ":"other"}],"active ":true,"budget ":125,"code ":"Argus-123","manager ":"baris","name ":"Argus"}])

        qWarning() << "***********";


        foreach(const QJsonValue & activities_value, activitiesArray)
        {

             if(activities_value.toObject().value("active ").toString() == "true") //check if the project is active.
             {
                  ui->project_combox->addItem(activities_value.toObject().value("code ").toString());
             }
        }

    }
    else
    {
        QMessageBox::critical(this, tr("Error"), file .errorString());
    }


}



ActivityDialog::~ActivityDialog()
{
    delete ui;
}

QList<QString> ActivityDialog::getProjectCodes()
{
    return projectCodes_list;
}

void ActivityDialog::updateCategories()
{

    QFile file;
    QJsonDocument doc;
    QJsonObject obj;
    QString data_json;
    QJsonValue value;
    QJsonValue val;

    file.setFileName("C:/Qt/activity.json");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        data_json = file.readAll();
        file.close();

        doc = doc.fromJson(data_json.toUtf8());

        obj = doc.object();

        qWarning() << obj; // QJsonObject({"Activities ":[{"SubActivities ":[{"code ":"database","code1 ":"other"}],"active ":true,"budget ":125,"code ":"Argus-123","manager ":"baris","name ":"Argus"}]})

        value = obj.value(QString("Activities "));

        qWarning() << "***********";

        qWarning() << value; // QJsonValue(array, QJsonArray([{"SubActivities ":[{"code ":"database","code1 ":"other"}],"active ":true,"budget ":125,"code ":"Argus-123","manager ":"baris","name ":"Argus"}]))

        QJsonArray activitiesArray = value.toArray();

        qWarning() << "***********";

        qWarning() << activitiesArray;  // QJsonArray([{"SubActivities ":[{"code ":"database","code1 ":"other"}],"active ":true,"budget ":125,"code ":"Argus-123","manager ":"baris","name ":"Argus"}])

        qWarning() << "***********";

        ui->category_combox->clear();

        projectCodes_list.clear(); // We reset the ProjectCodes_list. (Because because it's causing problems on ReviewReport)

        foreach(const QJsonValue & activities_value, activitiesArray)
        {

             if(activities_value.toObject().value("active ").toString() == "true") //check if the project is active.
             {
                 projectCodes_list.append(activities_value.toObject().value("code ").toString());


                  if(ui->project_combox->currentText() == activities_value.toObject().value("code ").toString()) //set the subcodes according to the code names
                  {
                      qWarning() << activities_value.toObject().value("SubActivities "); // QJsonValue(array, QJsonArray([{"code ":"database","code ":"other"}]))

                      QJsonArray subactivitiesArray = activities_value.toObject().value("SubActivities ").toArray();

                      qWarning() << subactivitiesArray; // QJsonArray([{"code ":"database","code ":"other"}])

                      foreach(const QJsonValue & subactivities_value, subactivitiesArray)
                      {
                          ui->category_combox->addItem(subactivities_value.toObject().value("code ").toString());
                      }
                  }

             }
        }

    }
    else
    {
        QMessageBox::critical(this, tr("Error"), file .errorString());
    }

}

void ActivityDialog::on_buttonBox_accepted()
{

    QFile file1;

    QJsonDocument doc; // Create empty page
    QJsonObject rootObj;
    QJsonObject obj; //The innermost objects
    QJsonObject root_obj; // The outermost objects
    QJsonArray objArray;
    QJsonArray objArray2;
    QString current_date;

    QString user_name = LoginDialog().getManagerName();

    current_date = QDate().currentDate().toString("-yyyy-MM");

    QString file_name = user_name+current_date+".json";

    file1.setFileName("C:/Qt/Activities/"+file_name);

    QStringList activity_list = MainWindow().getActivityList();

    activity_list.append(file_name); // add new file

    MainWindow().setActivityList(activity_list); //notifiying MainWindow

    if(!file1.open(QIODevice::ReadOnly | QIODevice:: Text))
    {
        QString project_name = ui->project_combox->currentText();
        QString category_name = ui->category_combox->currentText();
        const int time = ui->time_spinbox->value();
        QString description = ui->description_text->toPlainText();
        QString selected_day = MainWindow().getCalendarValue(); // get calendar value

        //Syncing local variables with json objects
        obj.insert("Description ",description);
        obj.insert("code ",project_name);
        obj.insert("date ",selected_day);
        obj.insert("subcode ",category_name);
        obj.insert("time ",time);

        objArray.append(obj); //add obj to objArray


        root_obj.insert("frozen ","false");

        root_obj.insert("Entries ",objArray); // İçerideki veriler ile dışarıdaki(Entries) verileri birleştiriyor.

        //root_obj.insert("accepted ",objArray2); // Accepted değerleri için gerekli.


        doc.setObject(root_obj); //set last version of doc
        QByteArray data_json1 = doc.toJson();
        QFile output("C:/Qt/Activities/"+file_name);
        if(output.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        {
            output.write(data_json1);
            output.close();
           QMessageBox::information(this,tr("No Error"),tr("Json is Saved successfully!"));
        }

        else
        {
            QMessageBox::critical(this,tr("Error"),output.errorString());
        }

    }
    else
    {

        QString  data_json;
        QJsonObject obj2;
        QJsonValue val;
        QString description2;
        QString code;
        QString date;
        QString subCode;
        int time1;


            data_json = file1.readAll();
            file1.close();

            // I synchronized the data in the form to local variables.
            QString project_name = ui->project_combox->currentText();
            QString category_name = ui->category_combox->currentText();
            const int time = ui->time_spinbox->value();
            QString description = ui->description_text->toPlainText();
            QString selected_date = MainWindow().getCalendarValue();



            doc = doc.fromJson(data_json.toUtf8());

            rootObj = doc.object();


            QString isFlag = rootObj["frozen "].toString();


            if(isFlag == "false")
            {

                QJsonArray rootArray  = rootObj["Entries "].toArray();

                if(!(file1.size() <= 1))
                {
                    for (auto jsonObj : rootArray)
                    {
                        val = jsonObj.toObject().value("Description ");
                        description2 = val.toString();

                        obj2.insert("Description ",description2);



                        val = jsonObj.toObject().value("code ");
                        code = val.toString();

                        obj2.insert("code ",code);



                        val = jsonObj.toObject().value("date ");
                        date = val.toString();

                        obj2.insert("date ",date);



                        val = jsonObj.toObject().value("subcode ");
                        subCode = val.toString();

                        obj2.insert("subcode ",subCode);



                        val = jsonObj.toObject().value("time ");
                        time1  = val.toInt();

                        obj2.insert("time ",time1);


                        objArray.append(obj2);

                    }
                }

                //Json objeleri ile yerel değişkenlerini eşitliyoruz.
                obj.insert("Description ",description);
                obj.insert("code ",project_name);
                obj.insert("date ",selected_date);
                obj.insert("subcode ",category_name);
                obj.insert("time ",time);



                objArray.append(obj); // İçerideki verileri arraye atıyor.


                root_obj.insert("frozen ",isFlag);

                root_obj.insert("Entries ",objArray); // İçerideki veriler ile dışarıdaki(Entries) verileri birleştiriyor.

                //root_obj.insert("accepted ",objArray2);


                doc.setObject(root_obj); //En son halini set ettik
                QByteArray data_json1 = doc.toJson();
                QFile output("C:/Qt/Activities/"+file_name);
                if(output.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
                {
                    output.write(data_json1);
                    output.close();
                    QMessageBox::information(this,tr("No Error"),tr("Json is Saved successfully!"));
                }

                else
                {
                    QMessageBox::critical(this,tr("Error"),output.errorString());
                }
            }
            else
            {
                QJsonArray rootArray  = rootObj["Entries "].toArray();


                qWarning() << file1.size();
                if(!(file1.size() <= 1))
                {
                    for (auto jsonObj : rootArray)
                    {
                        val = jsonObj.toObject().value("Description ");
                        description2 = val.toString();

                        obj2.insert("Description ",description2);



                        val = jsonObj.toObject().value("code ");
                        code = val.toString();

                        obj2.insert("code ",code);



                        val = jsonObj.toObject().value("date ");
                        date = val.toString();

                        obj2.insert("date ",date);



                        val = jsonObj.toObject().value("subcode ");
                        subCode = val.toString();

                        obj2.insert("subcode ",subCode);



                        val = jsonObj.toObject().value("time ");
                        time1  = val.toInt();

                        obj2.insert("time ",time1);


                        objArray.append(obj2);

                    }
                }

                /*for (int i  = 0;i < projectCodes_list.size(); i++)
                {
                 QJsonObject obj3;
                 obj3.insert("code ",projectCodes_list.at(i));
                 obj3.insert("time ",100);

                 objArray2.append(obj3);
                }*/

                root_obj.insert("frozen ","true");

                root_obj.insert("Entries ",objArray); // İçerideki veriler ile dışarıdaki(Entries) verileri birleştiriyor.

                //root_obj.insert("accepted ",objArray2);


                doc.setObject(root_obj); //En son halini set ettik
                QByteArray data_json1 = doc.toJson();
                QFile output("C:/Qt/Activities/"+file_name);
                if(output.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
                {
                    output.write(data_json1);
                    output.close();
                    //QMessageBox::information(this,tr("No Error"),tr("Json is Saved successfully!"));
                }

                else
                {
                    QMessageBox::critical(this,tr("Error"),output.errorString());
                }

                QMessageBox::information(this,tr("FLAG"),tr("Your activities set Flag! You cannot edit on your own activities!"));
            }

    }
}

