#ifndef MATERIALDETAILSDIALOG_H
#define MATERIALDETAILSDIALOG_H

#include <QtWidgets/QDialog>
#include <QtSql/QSqlQueryModel>

class QComboBox;
class QTableView;

class MaterialDetailsDialog : public QDialog {
    Q_OBJECT

public:
    MaterialDetailsDialog(int materialId, const QString &materialName, QWidget *parent = nullptr);

private slots:
    void onCharacteristicSelected(int index);

private:
    int materialId;
    QComboBox *characteristicsCombo;
    QTableView *dataTable;
    QTableView *sourceTable;
    QSqlQueryModel *dataModel;
    QSqlQueryModel *sourceModel;
    void loadCharacteristics();
    void loadData(int pairId);
};

#endif // MATERIALDETAILSDIALOG_H
