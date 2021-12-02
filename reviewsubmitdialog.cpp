#include "reviewsubmitdialog.h"
#include "ui_reviewsubmitdialog.h"
#include "mainwindow.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>

QString  mainCode;

reviewSubmitDialog::reviewSubmitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::reviewSubmitDialog)
{
    ui->setupUi(this);

    QStringList activity_list = MainWindow().getActivityList();

    qWarning() << "activity_list =>"<< activity_list;
    QString manager_name = LoginDialog().getManagerName();

    QJsonDocument doc;
    QJsonObject rootObj;
    QJsonObject obj;
    QString data_json;
    QJsonValue value;
    QJsonValue val;

    QFile file;
    file.setFileName("C:/Qt/activity.json");

    ui->activitiesComboBox->clear(); // In every opening clear the combobox



    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        data_json = file.readAll();
        file.close();

        doc = doc.fromJson(data_json.toUtf8());
        rootObj = doc.object();
        QJsonArray rootArray  = rootObj["Activities "].toArray();

        if(!(file.size() <= 1)) // Will not print read operation results if file is empty.
        {
            foreach(const QJsonValue & activities_value, rootArray) // read from file append to arrays
            {
                if(activities_value.toObject().value("manager ").toString() == manager_name)
                {
                    mainCode = activities_value.toObject().value("code ").toString();
                }
            }
        }
    }

    QFile file1;

    for (int i = 0; i < activity_list.size(); i++) // Do this until come to the end of the Activities directory.
    {
        file1.setFileName("C:/Qt/Activities/"+activity_list.at(i)); // Read Activities file.
        if(file1.open(QIODevice::ReadOnly | QIODevice::Text))
        {

            data_json = file1.readAll();
            file1.close();

            doc = doc.fromJson(data_json.toUtf8());
            rootObj = doc.object();

            QString isFlag = rootObj["frozen "].toString();

            if(isFlag == "true")
            {
                QJsonArray rootArray  = rootObj["Entries "].toArray();

                if(!(file.size() <= 1))
                {
                    for (auto jsonObj : rootArray)
                    {
                        if(jsonObj.toObject().value("code ") == mainCode)
                        {
                            qWarning() << "List"<<activity_list;
                            qWarning() << "code=>"<<mainCode;

                            ui->activitiesComboBox->addItem(activity_list.at(i));
                        }
                    }
                }
            }
        }
    }


    if(ui->activitiesComboBox->count() == 0)
    {
         this->close();
         QMessageBox::critical(this,tr("EMPTY COMBOBOX"),tr("You don't have any activities to set accepted values!"));
    }
}

reviewSubmitDialog::~reviewSubmitDialog()
{
    delete ui;
}

bool reviewSubmitDialog::checkTwiceEntry()
{
    QString file_name1 = ui->activitiesComboBox->currentText();
    QFile file1;

    QJsonDocument doc1;
    QJsonObject rootObj1;
    QString data_json1;
    QJsonValue value1;
    QJsonValue val1;
    QString code1;


    file1.setFileName("C:/Qt/Activities/"+file_name1);

    if(file1.open(QIODevice::ReadOnly | QIODevice:: Text))
    {
        data_json1 = file1.readAll();
        file1.close();

        doc1 = doc1.fromJson(data_json1.toUtf8());
        rootObj1 = doc1.object();

        QJsonArray rootArray  = rootObj1["accepted "].toArray();

        for (auto jsonObj : rootArray)
        {
            code1 = jsonObj.toObject().value("code ").toString();
            qWarning()<< "CHECK1"<<code1;
            qWarning()<< "CHECK2"<<mainCode;
            if(mainCode == code1)
            {
                this->close();
                QMessageBox::critical(this,tr("Twice Entry"),tr("You have alreay enter accepted time value!"));
                return true;
            }
        }
    }

    return false;

}

void reviewSubmitDialog::on_buttonBox_accepted()
{
    if(checkTwiceEntry())
    {
        this->close();
    }
    else
    {
        // set the accepted attributes for the selected activity
        QString file_name = ui->activitiesComboBox->currentText();
        QFile file;
        QJsonDocument doc;
        QJsonObject rootObj;
        QJsonObject obj;
        QJsonObject obj2;
        QJsonArray objArray2;
        QString data_json;
        QJsonValue value;
        QJsonValue val;
        int time;
        QString code;


        file.setFileName("C:/Qt/Activities/"+file_name);

        if(file.open(QIODevice::ReadOnly | QIODevice:: Text))
        {
            data_json = file.readAll();
            file.close();

            doc = doc.fromJson(data_json.toUtf8());
            rootObj = doc.object();


            QJsonArray rootArray  = rootObj["accepted "].toArray();

            for (auto jsonObj : rootArray)
            {
                code = jsonObj.toObject().value("code ").toString();

                obj2.insert("code ",code);

                qWarning()<< "code=>"<<code;

                time = jsonObj.toObject().value("time ").toInt();

                obj2.insert("time ",time);

                qWarning() << "time=>"<<time;

                objArray2.append(obj2);

            }




            QJsonObject obj3;


            time = ui->acceptedTimeSpinBox->value();


            obj3.insert("code ",mainCode);


            obj3.insert("time ",time);


            objArray2.append(obj3);


            qWarning()<< "Accepted2"<<objArray2;


            rootObj.insert("accepted ",objArray2);


            doc.setObject(rootObj);


            if(file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            {
                file.write(doc.toJson());
                file.close();

                QMessageBox::information(this,tr("No Error"),tr("Json is Saved successfully!"));
            }

        }
    }
}


