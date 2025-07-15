#include "materialdetailsdialog.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTableView>
#include <QtSql/QSqlQuery>

MaterialDetailsDialog::MaterialDetailsDialog(int materialId, const QString &materialName, QWidget *parent)
    : QDialog(parent), materialId(materialId),
      dataModel(new QSqlQueryModel(this)), sourceModel(new QSqlQueryModel(this)) {

    setWindowTitle("Характеристики: " + materialName);
    QVBoxLayout *layout = new QVBoxLayout(this);

    characteristicsCombo = new QComboBox;
    connect(characteristicsCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onCharacteristicSelected(int)));

    layout->addWidget(new QLabel("Выберите характеристику:"));
    layout->addWidget(characteristicsCombo);

    dataTable = new QTableView;
    dataTable->setModel(dataModel);
    layout->addWidget(new QLabel("Данные:"));
    layout->addWidget(dataTable);

    sourceTable = new QTableView;
    sourceTable->setModel(sourceModel);
    layout->addWidget(new QLabel("Источники:"));
    layout->addWidget(sourceTable);

    loadCharacteristics();
}

void MaterialDetailsDialog::loadCharacteristics() {
    QSqlQuery query;
    query.prepare("SELECT characteristic, pair_id "
                  "FROM specifications "
                  "WHERE metal_id = :metal_id");
    query.bindValue(":metal_id", materialId);
    query.exec();

    while (query.next()) {
        QString name = query.value(0).toString();
        int pairId = query.value(1).toInt();
        characteristicsCombo->addItem(name, pairId);
    }

    if (characteristicsCombo->count() > 0)
        onCharacteristicSelected(0);
}

void MaterialDetailsDialog::onCharacteristicSelected(int index) {
    int pairId = characteristicsCombo->itemData(index).toInt();
    loadData(pairId);
}

void MaterialDetailsDialog::loadData(int pairId) {
    dataModel->setQuery(
        "SELECT time, temperature, value "
        "FROM values "
        "WHERE pair_id = " + QString::number(pairId));

    sourceModel->setQuery(
        "SELECT source_name, application, spreadsheet, illustration, page "
        "FROM sources "
        "WHERE pair_id = " + QString::number(pairId));
}
