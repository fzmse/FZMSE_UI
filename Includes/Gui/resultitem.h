#include "Includes/InternalTypes/PDDBManagedObjectCompareResult.h"
#include <QList>
#include <QVariant>

class resultItem
{
public:
    resultItem(const QList<QVariant> &data,
                resultItem *parent = 0);
    resultItem(const QList<QVariant> &data,
               InternalTypes::PDDBManagedObjectCompareResult resultObj,
               resultItem *parent = 0);
    ~resultItem();

    void appendItem(resultItem *item);

    resultItem *item(int row);
    int itemCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    resultItem *parent();

private:
    QList<resultItem*> itemList;
    QList<QVariant> itemData;
    InternalTypes::PDDBManagedObjectCompareResult resultObj;
    resultItem *parentItem;

    void setData();
};
