#include "Includes/Gui/resultItemModel.h"

using namespace InternalTypes;

resultItemModel::resultItemModel(QObject *parent) : QAbstractItemModel(parent)
{

}

void resultItemModel::setResultVector(std::vector<PDDBManagedObjectCompareResult> resultList)
{
    for (std::vector<PDDBManagedObjectCompareResult>::iterator it = resultList.begin(); it != resultList.end(); ++it)
    {
        results.append(*it);
    }
}

void resultItemModel::setRoot()
{
    if (!results.empty())
    {
        QList<QVariant> rootData;
        rootData << "Type" << "Origin" << "Class";
        rootItem = new resultItem(rootData);
        //setupModelData(data.split(QString("\n")), rootItem);
    }
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
        return createIndex(row, column, childItem);
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

void resultItemModel::setupModelData(const QStringList &lines, resultItem *parent)
{
    QList<resultItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].mid(position, 1) != " ")
                break;
            position++;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QList<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->itemCount() > 0) {
                    parents << parents.last()->item(parents.last()->itemCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            //parents.last()->appendItem(new resultItem(columnData, parents.last()));
        }

        number++;
    }
}
