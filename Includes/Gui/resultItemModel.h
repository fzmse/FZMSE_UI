#pragma once

#include <QAbstractItemModel>
#include "Includes/Gui/resultItem.h"
#include "Includes/InternalTypes/PDDBManagedObjectCompareResult.h"
#include <QDebug>
#include <QIcon>
#include <vector>


class resultItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    resultItemModel(QObject *parent = 0);
    ~resultItemModel();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    resultItem * getItemFromRow(int row);
    resultItem * getRoot();
    void setResultVector(std::vector<InternalTypes::PDDBManagedObjectCompareResult> resultList);
    void setRoot();
    void clean();
    void selectNode();
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

private:
    resultItem * rootItem;
    QList<InternalTypes::PDDBManagedObjectCompareResult> results;
    void setupModelData();

};
