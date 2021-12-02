#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logindialog.h"
#include "reviewreportdialog.h"
#include "reviewsubmitdialog.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QStringListModel>
#include <QMessageBox>

QStringList activity_list;
QString calendar_value = QDate().currentDate().toString("yyyy-MM-dd "); // set calendar with today's day
QStringList manager_list;
QStringList active_project_list;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Time Reporting System");


    setUIVisibilityBeforeLogin();

    ui->tabWidget->setCurrentIndex(0); // Initialize first tab

    on_tabWidget_tabBarClicked(1); // doing this because we have to fill activity_list

    qWarning() << activity_list;

    LoginDialog *dialog = new LoginDialog(this); // opening login screen
    dialog->show();



    QStringListModel* active_model = new QStringListModel(this);
    QStringListModel* inactive_model = new QStringListModel(this);



    QStringList active_list;
    QStringList inactive_list;


    QJsonDocument doc;
    QJsonObject rootObj;
    QJsonObject obj;
    QString data_json;
    QString code;
    QString manager_name;


    QFile file;
    file.setFileName("C:/Qt/activity.json");


    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        data_json = file.readAll();
        file.close();

        doc = doc.fromJson(data_json.toUtf8());
        rootObj = doc.object();
        QJsonArray rootArray2  = rootObj["Activities "].toArray(); //QJsonObject subObj = rootArray[0].; // QJsonObject({"SubActivities ":[],"active ":"","budget ":0,"manager ":"","name ":""})

        if(!(file.size() <= 1)) //Dosya boşsa okuma işlemi sonuçlarını yazdırmayacak.
        {
            foreach(const QJsonValue & activities_value, rootArray2) // Dosyadan oku arraylere appendle
            {
                if(activities_value.toObject().value("active ") == "true")
                {
                    code = activities_value.toObject().value("code ").toString();

                    active_project_list.append(code);

                    manager_name = activities_value.toObject().value("manager ").toString();

                    manager_list.append(manager_name);

                    active_list.append(code);
                }
                else
                {
                    code = activities_value.toObject().value("code ").toString();

                    inactive_list.append(code);
                }

            }

            // set active an inactive ListViews

            inactive_model->setStringList(inactive_list);

            active_model->setStringList(active_list);

            ui->activeProjectsListView->setModel(active_model);

            ui->inactiveProjectsListView->setModel(inactive_model);

        }

    }

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_createNewActivityButton_clicked()
{
    MenuDialog *dialog = new MenuDialog(this);
    dialog->show();
}


void MainWindow::on_addNewButton_clicked()
{
    ActivityDialog *dialog = new ActivityDialog(this);
    dialog->show();
}

void MainWindow::on_selectDayButton_clicked()
{
     QString user_name = LoginDialog().getManagerName();
     QString file_name = user_name+QDate::currentDate().toString("-yyyy-MM")+".json";

     QStringListModel* currentActivities_model = new QStringListModel(this);

     QStringList currentActivities_list;

     currentActivities_model->setStringList(currentActivities_list); // set empty list if calendar value != date then show empty list

     ui->currentDayActivitiesListView->setModel(currentActivities_model);


     QFile file;
     QJsonDocument doc;
     QJsonObject obj;
     QJsonObject obj2;
     QString data_json;
     QJsonObject rootObj;
     QJsonValue val;
     QString description;
     QString code;
     QString activity;
     QString date;
     QString subcode;
     int time;

     file.setFileName("C:/Qt/Activities/"+file_name);

     if(file.open(QIODevice::ReadOnly | QIODevice::Text))
     {
         data_json = file.readAll();
         file.close();

         doc = doc.fromJson(data_json.toUtf8());

         rootObj = doc.object();

         QJsonArray rootArray  = rootObj["Entries "].toArray();
         qWarning() << rootArray;


             foreach(const QJsonValue & activities_value, rootArray) // read from file append to arrays
             {
                     date = activities_value.toObject().value("date ").toString();

                     if(calendar_value == date)
                     {
                         code = activities_value.toObject().value("code ").toString();

                         description = activities_value.toObject().value("Description ").toString();

                         time = activities_value.toObject().value("time ").toInt();

                         subcode =  activities_value.toObject().value("subcode ").toString();

                         activity = "Project:"+code+"=> Subcode: "+subcode+"=> Description: "+description+"=> Time: "+QString::number(time);

                         //Update currentActivitiesListView

                         currentActivities_list.append(activity);

                         currentActivities_model->setStringList(currentActivities_list);

                         ui->currentDayActivitiesListView->setModel(currentActivities_model);

                     }

             }

             // Also update currentDayActivitiesTitle
             ui->currentDayActivitiesTitle->setText("Your "+calendar_value+" Day Activities");


         ui->tabWidget->setCurrentIndex(2);


     }
}

void MainWindow::setUIVisibilityBeforeLogin()
{
    ui->loginButton->setVisible(true);
    ui->welcomeLabel->setVisible(true);
    ui->changeUser->setVisible(false);
    ui->currentDayActivitiesTitle->setVisible(false);
    ui->currentUserTitleLabel->setVisible(false);
    ui->currentUserLabel->setVisible(false);
    ui->tabWidget->setVisible(false);
    ui->totalTimeSpentTitle->setVisible(false);
    ui->addNewButton->setVisible(false);
    ui->createNewActivityButton->setVisible(false);
    ui->addNewButton->setVisible(false);
    ui->selectDayButton->setVisible(false);

}

void MainWindow::setUIVisibilityAfterLogin()
{
    ui->loginButton->setVisible(false);
    ui->welcomeLabel->setVisible(false);
    ui->changeUser->setVisible(true);
    ui->currentUserTitleLabel->setVisible(true);
    ui->currentUserLabel->setVisible(true);
    ui->tabWidget->setVisible(true);
    ui->currentDayActivitiesTitle->setVisible(true);
    ui->totalTimeSpentTitle->setVisible(true);
    ui->addNewButton->setVisible(true);
    ui->createNewActivityButton->setVisible(true);
    ui->selectDayButton->setVisible(true);

    ui->reviewButton->setDisabled(true);
    ui->activateButton->setDisabled(true);
    ui->inactivateButton->setDisabled(true);

    QString current_user = LoginDialog().getManagerName();

    ui->currentUserLabel->setText(current_user);
}

void MainWindow::decideManagerOrUser()
{
    QString current_user;
    current_user = LoginDialog().getManagerName();

    for (int i = 0;i<manager_list.size() ;i++ )
    {

        qWarning() << current_user;
        qWarning() << manager_list;
        if(manager_list.at(i) == current_user) // If an admin is equal to the current user, show the current user as admin.
        {
            qWarning()<< "manager"<<current_user;
            ui->currentUserLabel->setText(current_user+"(Manager)");
            break;
        }      
            qWarning() << "user"<<current_user;
            ui->currentUserLabel->setText(current_user+"(User)"); // If not show the current user as user.
    }
}


void MainWindow::on_refreshButton_clicked()
{
    QString current_user = LoginDialog().getManagerName();
    QStringListModel* model = new QStringListModel(this);
    QStringListModel* model1 = new QStringListModel(this);

    QStringList list;
    QStringList list1;

    QJsonDocument doc;
    QJsonObject rootObj;
    QJsonObject obj;
    QString data_json;
    QString code;

    bool isInList = false;


    QFile file;
    file.setFileName("C:/Qt/activity.json");



    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        data_json = file.readAll();
        file.close();

        doc = doc.fromJson(data_json.toUtf8());
        rootObj = doc.object();
        QJsonArray rootArray  = rootObj["Activities "].toArray(); //QJsonObject subObj = rootArray[0].; // QJsonObject({"SubActivities ":[],"active ":"","budget ":0,"manager ":"","name ":""})

        if(!(file.size() <= 1)) //Dosya boşsa okuma işlemi sonuçlarını yazdırmayacak.
        {
            active_project_list.clear();

            foreach(const QJsonValue & activities_value, rootArray) // Dosyadan oku arraylere appendle
            {
                if(activities_value.toObject().value("manager ").toString() == current_user)
                {
                    for (int i = 0;i<manager_list.size(); i++)
                    {
                        if(manager_list.at(i) == current_user)
                        {
                            isInList = true;
                        }
                    }
                    if(!isInList)
                    {
                        ui->currentUserLabel->setText(current_user+"(Manager)");
                        manager_list.append(current_user);
                    }
                }
                if(activities_value.toObject().value("active ") == "true")
                {
                    code = activities_value.toObject().value("code ").toString(); // add active projects to active_project_list and also add active ListView

                    active_project_list.append(code);

                    list.append(code);
                }
                else
                {
                    code = activities_value.toObject().value("code ").toString(); // add inactive projects to inactive ListView


                    list1.append(code);
                }

            }



            model->setStringList(list);

            ui->activeProjectsListView->setModel(model);


            model1->setStringList(list1);

            ui->inactiveProjectsListView->setModel(model1);

        }

    }

    for (int i = 0;i<manager_list.size() ; i++)
    {
        if(manager_list.at(i) == current_user)
        {
            ui->reviewButton->setDisabled(false);
            ui->activateButton->setDisabled(false);
            ui->inactivateButton->setDisabled(false);
        }
    }

}

void MainWindow::on_monthlyReport_clicked()
{
    ReviewReportDialog *dialog = new ReviewReportDialog(this);
    dialog->show();
}


void MainWindow::on_loginButton_clicked()
{
    setUIVisibilityAfterLogin();

    decideManagerOrUser();

    QString user_name = LoginDialog().getManagerName();
    for (int i = 0;i<manager_list.size() ; i++)
    {
        if(manager_list.at(i) == user_name)
        {
            ui->reviewButton->setDisabled(false);
            ui->activateButton->setDisabled(false);
            ui->inactivateButton->setDisabled(false);
        }
    }
}

QStringList MainWindow::getActivityList()
{
    return activity_list;
}

void MainWindow::setActivityList(QStringList new_activity_list)
{
    activity_list = new_activity_list;
}

QString MainWindow::getCalendarValue()
{
    return calendar_value;
}

void MainWindow::on_calendarWidget_selectionChanged()
{
    calendar_value = ui->calendarWidget->selectedDate().toString("yyyy-MM-dd ");


}

void MainWindow::on_changeUser_clicked()
{
    setUIVisibilityBeforeLogin();

    ui->tabWidget->setCurrentIndex(0);
    on_tabWidget_tabBarClicked(0);

    LoginDialog *dialog = new LoginDialog(this);

    dialog->show();

}


void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    if(index == 0)
    {
        QString user_name = LoginDialog().getManagerName();
        for (int i = 0;i<manager_list.size() ; i++)
        {
            qWarning() << user_name;
            qWarning() << manager_list.at(i);
            if(manager_list.at(i) == user_name)
            {
                ui->reviewButton->setDisabled(false);
                ui->activateButton->setDisabled(false);
                ui->inactivateButton->setDisabled(false);
            }
        }
    }
    if(index == 1)
    {
        activity_list.clear(); // clear the activity_list otherwise all values remain in the activity_list.
        QStringListModel* allActivities_model = new QStringListModel(this);
        QDir directory("C:/Qt/Activities");
        QStringList activityList = directory.entryList(QStringList() << "*.json" << "*.JSON",QDir::Files);
        foreach(QString filename, activityList) { // Returns the names of all files in the given Path.
            activity_list.append(filename);

        }

        allActivities_model->setStringList(activityList);
        ui->allActivitiesListView->setModel(allActivities_model);

    }
    if(index == 2)
    {

        QStringListModel* currentActivities_model = new QStringListModel(this);
        QStringListModel* totalTimeSpent_model = new QStringListModel(this);

        QStringList currentActivities_list;
        QStringList totalTimeSpent_list;

        QString data_json;
        QJsonDocument doc;
        QJsonObject rootObj;
        QString description;
        QString activity;
        QString date;
        QString code;
        int totalTimeSpentAllActivities = 0;
        QString subcode;
        int time;
        QString cur_date = QDate().currentDate().toString("yyyy-MM-dd ");

        QString user_name = LoginDialog().getManagerName();
        QString current_month = QDate().currentDate().toString("-yyyy-MM");

        QString activity_name = user_name+current_month+".json";


        QFile file2;

        QString file_name;

        for (int i = 0;i<activity_list.size() ;i++ )
        {
            if(activity_list.at(i) == activity_name)
            {
                file_name = activity_list.at(i);
            }
        }
        qWarning() << file_name;

        file2.setFileName("C:/Qt/Activities/"+file_name);

        if(file2.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            data_json = file2.readAll();
            file2.close();

            doc = doc.fromJson(data_json.toUtf8());
            rootObj = doc.object();
            QJsonArray rootArray1  = rootObj["Entries "].toArray(); //QJsonObject subObj = rootArray[0].; // QJsonObject({"SubActivities ":[],"active ":"","budget ":0,"manager ":"","name ":""})

            if(!(file2.size() <= 1)) // If the file is empty, the read operation will not print its results.
            {
                foreach(const QJsonValue & activities_value, rootArray1) // read from file append to arrays
                {
                        date = activities_value.toObject().value("date ").toString();

                        totalTimeSpentAllActivities += activities_value.toObject().value("time ").toInt();

                        qWarning() <<"Current Date:" <<cur_date;
                        if(cur_date == date)
                        {
                            code = activities_value.toObject().value("code ").toString();

                            description = activities_value.toObject().value("Description ").toString();

                            time = activities_value.toObject().value("time ").toInt();

                            subcode =  activities_value.toObject().value("subcode ").toString();

                            activity = "Project:"+code+"=> Subcode: "+subcode+"=> Description: "+description+"=> Time: "+QString::number(time);

                            currentActivities_list.append(activity);

                            currentActivities_model->setStringList(currentActivities_list);

                            ui->currentDayActivitiesListView->setModel(currentActivities_model);
                        }

                }

                 ui->currentDayActivitiesTitle->setText("Your Current Day Activities");

                 currentActivities_model->setStringList(currentActivities_list);

                 ui->currentDayActivitiesListView->setModel(currentActivities_model);

                totalTimeSpent_list.append("The total time you spend on all activities is "+QString::number(totalTimeSpentAllActivities));
                totalTimeSpent_model->setStringList(totalTimeSpent_list);

                ui->totalTimeSpentListView->setModel(totalTimeSpent_model);

            }

        }
        else
        {
            currentActivities_list.clear();
            currentActivities_model->setStringList(currentActivities_list);
            ui->currentDayActivitiesListView->setModel(currentActivities_model);

            totalTimeSpent_list.clear();
            totalTimeSpent_model->setStringList(totalTimeSpent_list);
            ui->totalTimeSpentListView->setModel(totalTimeSpent_model);

        }
    }
}


void MainWindow::on_reviewButton_clicked()
{
    reviewSubmitDialog *dialog = new reviewSubmitDialog(this); // opening login screen
    dialog->show();
}



void MainWindow::on_submitButton_clicked()
{
    QFile file;
    QString data_json;
    QJsonObject obj; // The innermost objects
    QJsonObject root_obj; // The outermost objects
    QJsonArray objArray;
    QJsonArray objArray2;
    QJsonObject obj2;
    QJsonValue val;
    QString description2;
    QString code;
    QString date;
    QString subCode;
    int time1;
    QJsonObject rootObj;
    QJsonDocument doc;
    QString user_name = LoginDialog().getManagerName();
    qWarning() << "USER NAME: "<<user_name;
    QString file_name = user_name+QDate::currentDate().toString("-yyyy-MM")+".json";
    file.setFileName("C:/Qt/Activities/"+file_name);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        data_json = file.readAll();
        file.close();

        doc = doc.fromJson(data_json.toUtf8());

        rootObj = doc.object();

        QString isFlag = rootObj["frozen "].toString();



        if(isFlag == "false")
        {
            QJsonArray rootArray  = rootObj["Entries "].toArray();


            if(!(file.size() <= 1))
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

            /*for (int i  = 0;i < active_project_list.size(); i++)
            {
             QJsonObject obj3;
             obj3.insert("code ",active_project_list.at(i));

             obj3.insert("time ",active_project_list.at(i));

             objArray2.append(obj3);
            }*/

            root_obj.insert("frozen ","true");

            root_obj.insert("Entries ",objArray); // It combines internal data with external (Entries) data.

            //root_obj.insert("accepted ",objArray2);




            doc.setObject(root_obj); // We set the latest version
            QByteArray data_json1 = doc.toJson();
            QFile output("C:/Qt/Activities/"+file_name);
            if(output.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
            {
                output.write(data_json1);
                output.close();
            }

            else
            {
                QMessageBox::critical(this,tr("Error"),output.errorString());
            }

            for (int i = 0;i<activity_list.size() ; i++)
            {
                if(activity_list.at(i) == file_name)
                {
                    activity_list.remove(i);
                }
            }

            QMessageBox::information(this,tr("FLAG"),tr("Your activities set Flag! You cannot edit on your own activities anymore!"));
        }
        else
        {
            QMessageBox::critical(this,tr("FLAG"),tr("You have already submit your activities!"));
        }

    }
    else
    {
        QMessageBox::critical(this,tr("Error"),tr("You don't have any activities, so you cannot submit!"));
    }

}
    //Whichever user pressed submit, that user activity is now inaccessible.


void MainWindow::on_inactivateButton_clicked()
{
   QModelIndex index = ui->activeProjectsListView->currentIndex();

   QString itemIndex = index.data(Qt::DisplayRole).toString();

   QFile file;
   QJsonDocument doc;
   QJsonObject rootObj;
   QJsonObject obj2;
   QJsonObject subCode;
   QJsonArray JsonSubArray2;
   QJsonArray JsonArray;
   QString data_json;

   QString code;
   QString  mainCode;
   int budget;
   QString manager;
   QString name;
   QString active;

   file.setFileName("C:/Qt/activity.json");

   if(file.open(QIODevice::ReadOnly | QIODevice::Text))
   {
       data_json = file.readAll();
       file.close();

       doc = doc.fromJson(data_json.toUtf8());
       rootObj = doc.object();
       QJsonArray rootArray  = rootObj["Activities "].toArray();

       if(!(file.size() <= 1)) //If the file is empty, the read operation will not print its results.
       {
           foreach(const QJsonValue & activities_value, rootArray) // read from file append to arrays
           {

               if(activities_value.toObject().value("code ").toString() == itemIndex)
               {

                   obj2.insert("active ","false"); // Change the project activity to false.
               }
               else
               {
                   active = activities_value.toObject().value("active ").toString();
                   obj2.insert("active ",active); // Do normal things
               }

              QJsonArray subactivitiesArray = activities_value.toObject().value("SubActivities ").toArray();



               foreach(const QJsonValue & subactivities_value, subactivitiesArray)
               {

                   code = subactivities_value.toObject().value("code ").toString();



                   subCode.insert("code ",code);

                   JsonSubArray2.append(subCode); // Data is accumulating in Array.


               }

               obj2.insert("SubActivities ",JsonSubArray2);

               JsonSubArray2 = QJsonArray();



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

           rootObj.insert("Activities ", JsonArray);

           doc.setObject(rootObj);


           if(file.open(QIODevice::WriteOnly | QIODevice::Truncate))
           {
               file.write(doc.toJson());
               file.close();
           }

       }


   }

   on_refreshButton_clicked();

    //Pass the elements in the activate list to inactivate.(Change the activity.json.)
}


void MainWindow::on_activateButton_clicked()
{
    QModelIndex index = ui->inactiveProjectsListView->currentIndex();

    QString itemIndex = index.data(Qt::DisplayRole).toString();
    qWarning() <<"selecte item: " <<itemIndex;

    QFile file;
    QJsonDocument doc;
    QJsonObject rootObj;
    QJsonObject obj2;
    QJsonObject subCode;
    QJsonArray JsonSubArray2;
    QJsonArray JsonArray;
    QString data_json;

    QString code;
    QString  mainCode;
    int budget;
    QString manager;
    QString name;
    QString active;

    file.setFileName("C:/Qt/activity.json");

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        data_json = file.readAll();
        file.close();

        doc = doc.fromJson(data_json.toUtf8());
        rootObj = doc.object();
        QJsonArray rootArray  = rootObj["Activities "].toArray();

        if(!(file.size() <= 1)) //Dosya boşsa okuma işlemi sonuçlarını yazdırmayacak.
        {
            foreach(const QJsonValue & activities_value, rootArray) // Dosyadan oku arraylere appendle
            {

                if(activities_value.toObject().value("code ").toString() == itemIndex)
                {

                    obj2.insert("active ","true"); // Change the project activity to true.
                }
                else
                {
                    active = activities_value.toObject().value("active ").toString();
                    obj2.insert("active ",active);
                }



               QJsonArray subactivitiesArray = activities_value.toObject().value("SubActivities ").toArray();



                foreach(const QJsonValue & subactivities_value, subactivitiesArray)
                {

                    code = subactivities_value.toObject().value("code ").toString();



                    subCode.insert("code ",code);

                    JsonSubArray2.append(subCode);



                }

                obj2.insert("SubActivities ",JsonSubArray2);

                JsonSubArray2 = QJsonArray();



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

            rootObj.insert("Activities ", JsonArray);

            doc.setObject(rootObj);


            if(file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            {
                file.write(doc.toJson());
                file.close();
            }

        }


    }
    // Pass the elements in the inactivate list to activate. (Change activity.json.)
}
