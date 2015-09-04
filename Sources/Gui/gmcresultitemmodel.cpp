
#include "Includes/Gui/gmcResultItemModel.h"

using namespace InternalTypes;

gmcResultItemModel::gmcResultItemModel(appGUI * appGui ,QObject *parent) : QAbstractItemModel(parent)
{
    app = appGui;
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

void gmcResultItemModel::changeIncludeInGMC(const QModelIndex & index)
{
    auto item = getItemFromRow(index.row());
    int id = item->resultObj.getPDDBCompareResultId();
    for (int i = 0; i < results.size(); i++)
    {
        if ( results[i].getPDDBCompareResultId() == id )
        {
            auto actions = app->getActions();
            if ( !item->resultObj.isIncludedInGMC() )
            {
                (*actions)[i].setIncludedInGMC(true);
                item->resultObj.setIncludedInGMC(true);
                item->updateIncludedInGMC(true);
                break;
            } else
            {
                (*actions)[i].setIncludedInGMC(false);
                item->resultObj.setIncludedInGMC(false);
                item->updateIncludedInGMC(false);
                break;
            }
        }
    }
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

    gmcResultItem *item = static_cast<gmcResultItem*>(index.internalPointer());

    if ( index.column() == 0 )
        return static_cast< int >( item->isChecked() ? Qt::Checked : Qt::Unchecked );

    if ( index.column() == 3 && role == Qt::DecorationRole)
    {
        auto op = item->data(3).toString();
        if ( op == "Add" )
            return QIcon(":/report/add.png");
        if ( op == "Delete" )
            return QIcon(":/report/del.png");
        if ( op == "Modify" )
            return QIcon(":/report/mod.png");
    }

    if (role != Qt::DisplayRole)
        return QVariant();

    return item->data(index.column());
}

Qt::ItemFlags gmcResultItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if ( index.column() == 0  )
        flags |= Qt::ItemIsUserCheckable;


    return flags;
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

bool gmcResultItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ( index.isValid())
    {
        return false;
    }
    else
    {
        if (role == Qt::CheckStateRole)
        {
            changeIncludeInGMC(index);

        }
    }
    return false;
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
    rootData << " " << " " << "Type" << " " << "Location" << "Changes";

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
