#pragma once

#include "Includes/InternalTypes/PDDBManagedObjectCompareResult.h"
#include "Includes/InternalTypes/PDDBManagedObjectParameter.h"
#include <QList>
#include <QDebug>
#include <QVariant>

class resultItem
{
public:
    resultItem(const QList<QVariant> &data,
                resultItem *parent = 0);
    resultItem(InternalTypes::PDDBManagedObjectCompareResult resultObj,
               resultItem *parent = 0);
    ~resultItem();

    void appendItem(resultItem *item);
    void setParetn(resultItem *item);
    QString getLocation();
    resultItem *item(int row);
    int itemCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    resultItem * parent();
    resultItem * findItemById(int id);
    InternalTypes::PDDBManagedObjectCompareResult resultObj;

private:
    QList<resultItem*> itemList;
    QList<QVariant> itemData;
    resultItem *parentItem;

    void setData();
};
