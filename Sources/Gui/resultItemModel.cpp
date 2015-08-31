#include "Includes/Gui/resultItemModel.h"

using namespace InternalTypes;

resultItemModel::resultItemModel(QObject *parent) : QAbstractItemModel(parent)
{

}

resultItemModel::~resultItemModel()
{
    if (rootItem != NULL)
    {
        delete rootItem;
    }
}

void resultItemModel::setResultVector(std::vector<PDDBManagedObjectCompareResult> resultList)
{
    int i = 0;
    if (!results.empty())
        results.clear();

    for (std::vector<PDDBManagedObjectCompareResult>::iterator it = resultList.begin(); it != resultList.end(); ++it)
    {
        results.append(*it);
        ++i;
    }
    qDebug() << i << " PDDB results loaded";
}

void resultItemModel::setRoot()
{
    setupModelData();
}

void resultItemModel::clean()
{
    results.clear();
    rootItem = NULL;
}

int resultItemModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<resultItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant resultItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    resultItem *item = static_cast<resultItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags resultItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant resultItemModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex resultItemModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    resultItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<resultItem*>(parent.internalPointer());

    resultItem *childItem = parentItem->item(row);
    if (childItem)
    {
        return createIndex(row, column, childItem);
    }
    else
        return QModelIndex();
}


QModelIndex resultItemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    resultItem *childItem = static_cast<resultItem*>(index.internalPointer());
    resultItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int resultItemModel::rowCount(const QModelIndex &parent) const
{
    resultItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<resultItem*>(parent.internalPointer());

    return parentItem->itemCount();
}

void resultItemModel::setupModelData()
{

    QList<QVariant> rootData;
    rootData << "Type" << "Origin" << "Location" << "Changes";


    rootItem = new resultItem(rootData);
    rootItem->setParetn(rootItem);

    QListIterator<PDDBManagedObjectCompareResult> i(results);
    while (i.hasNext())
    {
        resultItem * item = new resultItem(i.next(), rootItem);
        rootItem->appendItem(item);
    }

}

resultItem * resultItemModel::getItemFromRow(int row)
{
    return rootItem->item(row);
}

resultItem * resultItemModel::getRoot()
{
    return rootItem;
}
