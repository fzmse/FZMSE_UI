
#include "Includes/Gui/gmcResultItemModel.h"

using namespace InternalTypes;

gmcResultItemModel::gmcResultItemModel(QObject *parent) : QAbstractItemModel(parent)
{

}

gmcResultItemModel::~gmcResultItemModel()
{
    if (rootItem != NULL)
    {
        delete rootItem;
    }
}

void gmcResultItemModel::setResultVector(std::vector<GMCAction> resultList)
{
    int i = 0;
    if (!results.empty())
        results.clear();

    for (std::vector<GMCAction>::iterator it = resultList.begin(); it != resultList.end(); ++it)
    {
        results.append(*it);
        ++i;
    }
    qDebug() << i << " GMC Actions Detected";
}

void gmcResultItemModel::setRoot()
{
    setupModelData();
}

void gmcResultItemModel::clean()
{
    results.clear();
    rootItem = NULL;
}

int gmcResultItemModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<gmcResultItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant gmcResultItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    gmcResultItem *item = static_cast<gmcResultItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags gmcResultItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant gmcResultItemModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex gmcResultItemModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    gmcResultItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<gmcResultItem*>(parent.internalPointer());

    gmcResultItem *childItem = parentItem->item(row);
    if (childItem)
    {
        return createIndex(row, column, childItem);
    }
    else
        return QModelIndex();
}


QModelIndex gmcResultItemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    gmcResultItem *childItem = static_cast<gmcResultItem*>(index.internalPointer());
    gmcResultItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int gmcResultItemModel::rowCount(const QModelIndex &parent) const
{
    gmcResultItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<gmcResultItem*>(parent.internalPointer());

    return parentItem->itemCount();
}

void gmcResultItemModel::setupModelData()
{
    QList<QVariant> rootData;
    rootData << " " << "Type" << "Origin" << "Location" << "Changes";

    rootItem = new gmcResultItem(rootData);
    rootItem->setParetn(rootItem);

    QListIterator<GMCAction> i(results);
    while (i.hasNext())
    {
        gmcResultItem * item = new gmcResultItem(i.next(), rootItem);
        rootItem->appendItem(item);

        if ( item->resultObj.getChangeScope() == GMCAction::ManagedObject && item->resultObj.getActionType() == GMCAction::Add)
        {
            auto children = item->resultObj.getChildActions();
            qDebug() << "Children cound "<< children.size();
            for (vector<GMCAction>::iterator it = children.begin(); it != children.end(); it++)
            {
                gmcResultItem * child = new gmcResultItem((*it), item);
                item->appendItem(child);
            }
        }
    }
}

gmcResultItem * gmcResultItemModel::getItemFromRow(int row)
{
    return rootItem->item(row);
}
