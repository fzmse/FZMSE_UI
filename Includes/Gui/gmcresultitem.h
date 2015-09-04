#pragma once

#include "Includes/InternalTypes/gmcaction.h"
#include "Includes/InternalTypes/gmcmanagedobjectparameter.h"
#include "Includes/InternalTypes/PDDBManagedObjectParameter.h"
#include <QList>
#include <Qdebug>
#include <QVariant>

class gmcResultItem
{
public:
    gmcResultItem(const QList<QVariant> &data,
                gmcResultItem *parent = 0);
    gmcResultItem(InternalTypes::GMCAction resultObj,
               gmcResultItem *parent = 0);
    ~gmcResultItem();

    void appendItem(gmcResultItem *item);
    void setParetn(gmcResultItem *item);
    void updateIncludedInGMC(bool state);
    bool isChecked();
    QString getLocation();
    gmcResultItem *item(int row);
    int itemCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    gmcResultItem *parent();
    InternalTypes::GMCAction resultObj;

private:
    QList<gmcResultItem*> itemList;
    QList<QVariant> itemData;
    gmcResultItem *parentItem;
    void setData();
};
