
#include "Includes/Gui/resultItem.h"

resultItem::resultItem(const QList<QVariant> &data,
                           resultItem *parent)
{
    this->parentItem = parent;
    this->itemData = data;
}

resultItem::resultItem(const QList<QVariant> &data,
                       InternalTypes::PDDBManagedObjectCompareResult resultObj,
                       resultItem *parent)
{
    this->resultObj = resultObj;
    this->parentItem = parent;

    if (data.empty())
        setData();
    else
        this->itemData = data;
}

resultItem::~resultItem()
{
    qDeleteAll(itemList);
}

void resultItem::appendItem(resultItem *item)
{
    itemList.append(item);
}

resultItem * resultItem::item(int row)
{
    return itemList.value(row);
}

int resultItem::itemCount() const
{
    return itemList.count();
}

int resultItem::columnCount() const
{
    return itemData.count();
}

QVariant resultItem::data(int column) const
{
    return itemData.value(column);
}

resultItem * resultItem::parent()
{
    return parentItem;
}

int resultItem::row() const
{
    if (parentItem)
        return parentItem->itemList.indexOf(const_cast<resultItem*>(this));

    return 0;
}

void resultItem::setData()
{
    std::string className = XmlElementReader::getAttributeByName(this->resultObj.getFirstElement()->getElement(),"class");
    QVariant qVarClassName;
    qVarClassName.setValue(QString::fromStdString(className));
//    QVariant QVarOrigin;
//    QVarOrigin.setValue(this->resultObj.getOrigin());
    itemData << qVarClassName;
}
