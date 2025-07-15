#include "mainwindow.h"
#include "materialdetailsdialog.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QSettings>
#include <QFile>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), materialsModel(new QSqlQueryModel(this)) {
    setupDatabase();
    setupUI();
    loadClasses();
    loadMaterials();
}

MainWindow::~MainWindow() {
    db.close();
}

void MainWindow::setupDatabase() {
    db = QSqlDatabase::addDatabase("QPSQL");
    QSettings config("D:\\test\\ciclika2\\connection.ini", QSettings::IniFormat);

    if (!QFile::exists("D:\\test\\ciclika2\\connection.ini"))
    {
            QMessageBox::critical(this, "Ошибка", "Файл connection.ini не найден!");
            exit(EXIT_FAILURE);
    }

    db.setHostName(config.value("database/host").toString());
    db.setPort(config.value("database/port").toInt());
    db.setDatabaseName(config.value("database/name").toString());
    db.setUserName(config.value("database/user").toString());
    db.setPassword(config.value("database/password").toString());

    if (!db.open()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к базе данных:\n" + db.lastError().text());
        exit(1);
    }
}

void MainWindow::setupUI() {
    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    QHBoxLayout *filterLayout = new QHBoxLayout;
    filterLayout->addWidget(new QLabel("Фильтр по классу:"));
    classFilter = new QComboBox;
    connect(classFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(onClassFilterChanged(int)));
    filterLayout->addWidget(classFilter);

    mainLayout->addLayout(filterLayout);

    materialsTable = new QTableView;
    materialsTable->setModel(materialsModel);
    materialsTable->horizontalHeader()->setStretchLastSection(true);
    materialsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    materialsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(materialsTable, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onMaterialButtonClicked()));

    mainLayout->addWidget(materialsTable);
    setCentralWidget(central);
    setWindowTitle("Список материалов");
}

void MainWindow::loadClasses() {
    classFilter->addItem("Все классы", -1);
    QSqlQuery query("SELECT class_id, class_name FROM classes ORDER BY class_name");
    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        classFilter->addItem(name, id);
    }
}

void MainWindow::loadMaterials(int classId) {
    QString sql =
        "SELECT m.metal_id, m.metal_name, m.assortment "
        "FROM metals m ";
    if (classId >= 0) {
        sql += "JOIN class_metals c ON c.metal_id = m.metal_id "
               "WHERE c.class_id = " + QString::number(classId);
    }
    materialsModel->setQuery(sql);
    materialsModel->setHeaderData(1, Qt::Horizontal, "Материал");
    materialsModel->setHeaderData(2, Qt::Horizontal, "Сортамент");
}

void MainWindow::onClassFilterChanged(int index) {
    int classId = classFilter->itemData(index).toInt();
    loadMaterials(classId);
}

void MainWindow::onMaterialButtonClicked() {
    QModelIndex index = materialsTable->currentIndex();
    if (!index.isValid()) return;

    int row = index.row();
    int materialId = materialsModel->data(materialsModel->index(row, 0)).toInt();
    QString materialName = materialsModel->data(materialsModel->index(row, 1)).toString();

    MaterialDetailsDialog dialog(materialId, materialName, this);
    dialog.exec();
}
