#include "menudialog.h"
#include "ui_menudialog.h"
#include "logindialog.h"
#include "reviewreportdialog.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMessageBox>
#include <QJsonArray>
#include <QStringListModel>

QStringList list;

MenuDialog::MenuDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MenuDialog)
{
    ui->setupUi(this);

    setWindowTitle("Create New Project");

}

MenuDialog::~MenuDialog()
{
    delete ui;
}

void MenuDialog::on_buttonBox_accepted()
{





    QJsonDocument doc;
    QJsonObject rootObj;
    QJsonObject obj;
    QJsonObject obj2;
    QJsonObject subCode;
    QJsonObject subCode2;
    QJsonObject subCode3;
    QJsonObject subActObj1;
    QJsonObject subActObj2;
    QJsonObject subActObj3;
    QJsonArray JsonSubArray;
    QJsonArray JsonSubArray2;
    QJsonArray JsonArray;
    QString data_json;
    QJsonValue value;
    QJsonValue val;

    QFile file;
    file.setFileName("C:/Qt/activity.json");



    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        data_json = file.readAll();
        file.close();

        doc = doc.fromJson(data_json.toUtf8());
        rootObj = doc.object();
        QJsonArray rootArray  = rootObj["Activities "].toArray();


        QString code;
        QString  mainCode;
        QString activity;
        int budget;
        QString manager;
        QString name;



        QString project_code = ui->project_codeLineEdit->text();

        QString project_manager = LoginDialog().getManagerName();

        QString project_name = ui->project_nameLineEdit->text();

        const int project_budget = ui->project_budget_spinBox->value();

        bool isActive = ui->checkBox->isChecked();

        QString Active = QVariant(isActive).toString();

        if(!(file.size() <= 1)) // If the file is empty, the read operation will not print its results.
        {
            foreach(const QJsonValue & activities_value, rootArray) // read from file append to arrays.
            {

                QJsonArray subactivitiesArray = activities_value.toObject().value("SubActivities ").toArray();



                foreach(const QJsonValue & subactivities_value, subactivitiesArray)
                {

                    code = subactivities_value.toObject().value("code ").toString();



                    subCode.insert("code ",code); // Try to create a solution with ComboBox.

                    JsonSubArray2.append(subCode); // Data is accumulating in Array.



                }

                obj2.insert("SubActivities ",JsonSubArray2);

                JsonSubArray2 = QJsonArray();




                activity = activities_value.toObject().value("active ").toString();

                obj2.insert("active ",activity);


                budget = activities_value.toObject().value("budget ").toInt();

                 obj2.insert("budget ",budget);




                mainCode = activities_value.toObject().value("code ").toString();

                obj2.insert("code ",mainCode);




                 manager = activities_value.toObject().value("manager ").toString();

                 obj2.insert("manager ",manager);



                 name = activities_value.toObject().value("name ").toString();

                 obj2.insert("name ",name);




                JsonArray.append(obj2);

            }

        }

        for (int i = 0; i<list.size() ;i++ )
        {
            subActObj1.insert("code ",list.at(i));
            JsonSubArray.append(subActObj1);
        }

        obj.insert("code ",project_code);

        obj.insert("manager ",project_manager);

        obj.insert("name ",project_name);

        obj.insert("budget ",project_budget);

        obj.insert("active ",Active);

        obj.insert("SubActivities ", JsonSubArray);

        JsonArray.append(obj);


        rootObj.insert("Activities ", JsonArray);

        doc.setObject(rootObj);


        if(file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            file.write(doc.toJson());
            file.close();
        }

        list.clear();

    }
    else
    {

         QFile file1;
         QJsonDocument doc;
         QJsonObject rootObj;
         QJsonObject obj;
         QJsonObject obj2;
         QJsonObject subCode;
         QJsonObject subCode2;
         QJsonObject subCode3;
         QJsonObject subActObj1;
         QJsonObject subActObj2;
         QJsonObject subActObj3;
         QJsonArray JsonSubArray;
         QJsonArray JsonArray;
         QJsonValue value;
         QJsonValue val;

         QString project_code = ui->project_codeLineEdit->text();

         QString project_manager = LoginDialog().getManagerName();

         QString project_name = ui->project_nameLineEdit->text();

         const int project_budget = ui->project_budget_spinBox->value();

         bool isActive = ui->checkBox->isChecked();

         QString Active = QVariant(isActive).toString();

         for (int i = 0; i<list.size() ;i++ )
         {
             subActObj1.insert("code ",list.at(i));
             JsonSubArray.append(subActObj1);
         }

         obj.insert("code ",project_code);

         obj.insert("manager ",project_manager);

         obj.insert("name ",project_name);

         obj.insert("budget ",project_budget);

         obj.insert("active ",Active);

         obj.insert("SubActivities ", JsonSubArray);

         JsonArray.append(obj);


         rootObj.insert("Activities ", JsonArray);

         doc.setObject(rootObj);

         file1.setFileName("C:/Qt/activity.json");


        if(file1.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            file1.write(doc.toJson());
            file1.close();
        }

        list.clear();

        QMessageBox::information(this,tr("No Error"),tr("Json is Read successfully!"));
    }
}

void MenuDialog::on_addSubactivityButton_clicked()
{
    QStringListModel* model = new QStringListModel(this);

    QString subAct =  ui->subActivity1_lineEdit->text();

    list.append(subAct);

    model->setStringList(list);

    ui->subActivitiesListView->setModel(model);

    ui->subActivity1_lineEdit->clear();

}

