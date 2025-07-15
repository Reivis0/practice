#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
        ui->setupUi(this);
        checkAllInfo();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkAllInfo()
{
    if(!connectToDatabase())
    {
        qDebug("ERROR: connection faild");
        db.close();
        exit(EXIT_FAILURE);
    }
    else
    {

        QString q = "SELECT "
                "c.name AS camp_name, "
                "c.country, "
                "c.city, "
                "d.director, "
                "d.birth_year, "
                "d.death_year, "
                "d.birth_place, "
                "ci.foundation_year, "
                "ci.total_employees, "
                "ci.total_prisoners "
            "FROM camps c "
            "LEFT JOIN "
                "director_info d ON c.director = d.director "
            "LEFT JOIN "
                "camp_info ci ON c.name = ci.name;";
        doSelectQuery(q);
        db.close();
    }
}

bool MainWindow::connectToDatabase()
{
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setPort(5432);
    db.setDatabaseName("Concentration_camps");
    db.setUserName("postgres");
    db.setPassword("1qaz@WSX");

    if (!db.open())
    {
        return false;
    }
    else
    {
     return true;
    }
}

void MainWindow::doSelectQuery(const QString& q) {
    QSqlQuery query;
    if (!query.exec(q)) {
        qDebug() << "Ошибка SQL:" << query.lastError().text();
        return;
    }
    ui->tableView->setModel(new QSqlQueryModel(this));
    ((QSqlQueryModel*)ui->tableView->model())->setQuery(query);

     ui->tableView->resizeColumnsToContents();
}

void MainWindow::on_pushButton_clicked()
{
    if(!connectToDatabase())
    {
        qDebug("ERROR: connection faild");
        db.close();
        exit(EXIT_FAILURE);
    }
    else
    {

        QString q = "SELECT "
                "name, "
                "country, "
                "city, "
                "director "
            "FROM camps;";

        doSelectQuery(q);
        db.close();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if(!connectToDatabase())
    {
        qDebug("ERROR: connection faild");
        db.close();
        exit(EXIT_FAILURE);
    }
    else
    {

        QString q = "SELECT "
                "name, "
                "foundation_year, "
                "total_employees, "
                "total_prisoners "
                "FROM camp_info;";

        doSelectQuery(q);
        db.close();
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    if(!connectToDatabase())
    {
        qDebug("ERROR: connection faild");
        db.close();
        exit(EXIT_FAILURE);
    }
    else
    {

        QString q = "SELECT "
                "director, "
                "birth_year, "
                "death_year, "
                "birth_place "
                "FROM director_info;";

        doSelectQuery(q);
        db.close();
    }
}


void MainWindow::on_pushButton_4_clicked()
{
    checkAllInfo();
}
