#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTableView>
#include <QtWidgets/QPushButton>
#include <QtSql/QSqlQueryModel>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onClassFilterChanged(int index);
    void onMaterialButtonClicked();

private:
    QSqlDatabase db;
    QComboBox *classFilter;
    QTableView *materialsTable;
    QSqlQueryModel *materialsModel;
    void setupDatabase();
    void setupUI();
    void loadClasses();
    void loadMaterials(int classId = -1);
};

#endif // MAINWINDOW_H
