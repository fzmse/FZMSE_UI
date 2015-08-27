
#include "Includes/Gui/gmcResultItem.h"

using namespace InternalTypes;


gmcResultItem::gmcResultItem(const QList<QVariant> &data,
                           gmcResultItem *parent)
{
    this->parentItem = parent;
    this->itemData = data;
}

gmcResultItem::gmcResultItem(GMCAction resultObj,
                       gmcResultItem *parent)
{
    this->resultObj = resultObj;
    this->parentItem = parent;
    setData();
}

gmcResultItem::~gmcResultItem()
{
    if (!itemList.empty())
        qDeleteAll(itemList);
}

void gmcResultItem::appendItem(gmcResultItem *item)
{
    itemList.append(item);
}

gmcResultItem * gmcResultItem::item(int row)
{
    return itemList.value(row);
}

int gmcResultItem::itemCount() const
{
    return itemList.count();
}

int gmcResultItem::columnCount() const
{
    return itemData.count();
}

QVariant gmcResultItem::data(int column) const
{
    return itemData.value(column);
}

gmcResultItem * gmcResultItem::parent()
{
    return parentItem;
}

void gmcResultItem::setParetn(gmcResultItem *item)
{
    parentItem = item;
}

QString gmcResultItem::getLocation()
{
    return itemData.value(2).toString();
}

int gmcResultItem::row() const
{
    if (parentItem)
        return parentItem->itemList.indexOf(const_cast<gmcResultItem*>(this));

    return 0;
}

inline QString stdToQ(std::string text)
{
    return QString::fromStdString(text);
}

inline void addStdToQ(QString &desc, std::string text)
{
    desc.append(stdToQ(text));
}

void gmcResultItem::setData()
{
    QString d;
    GMCAction r = resultObj;

    if ( r.isReaderInteractionRequired() )
        itemData << "[!]";

    else
        itemData << "  ";
    switch(r.getActionType())
    {
    case GMCAction::Add:
        if ( r.getChangeScope() == GMCAction::ManagedObjectParameter )
        {

            itemData << "Parameter" << "Add";
            addStdToQ(d, "[ ");
            if ( ((PDDBManagedObjectParameter*)r.getItem())->getMocParent() != NULL )
            {
                PDDBManagedObject* mocObj = (PDDBManagedObject*)((PDDBManagedObjectParameter*)r.getItem())->getMocParent();
                addStdToQ(d, mocObj->getClassName());
                addStdToQ(d, "->");
            }
            addStdToQ(d, XmlElementReader::getAttributeByName(r.getItem()->getElement(), "name"));
            addStdToQ(d, "]");

        }
        else if ( r.getChangeScope() == GMCAction::ManagedObject )
        {
            itemData << "Managed Object" << "Add";
            addStdToQ(d, "[ ");
            addStdToQ(d,((PDDBManagedObject*)r.getItem())->getClassName());
            addStdToQ(d, " ]");

        }
        else if ( r.getChangeScope() == GMCAction::ComplexParameter )
        {
            itemData << "Parameter" << "Add";
            addStdToQ(d, "[ ");
            addStdToQ(d,((PDDBManagedObject*)((PDDBManagedObjectParameter*)r.getItem())->getMocParent() )->getClassName());
            addStdToQ(d, " -> ");
            addStdToQ(d,((PDDBManagedObjectParameter*)r.getItem())->getName());
            addStdToQ(d, " ]");
        }

        break;
    case GMCAction::Remove:
        if ( r.getChangeScope() == GMCAction::ManagedObjectParameter )
        {
            itemData << "Parameter" << "Del";
            addStdToQ(d, "[ ");
            if ( ((PDDBManagedObjectParameter*)r.getItem())->getMocParent() != NULL )
            {
                PDDBManagedObject* mocObj = (PDDBManagedObject*)((PDDBManagedObjectParameter*)r.getItem())->getMocParent();
                addStdToQ(d, mocObj->getClassName());
                addStdToQ(d, "->");
            }
            addStdToQ(d, XmlElementReader::getAttributeByName(r.getItem()->getElement(), "name"));
            addStdToQ(d, "]");
        }

        if ( r.getChangeScope() == GMCAction::ManagedObject )
        {
            itemData << "Managed Object" << "Del";
            addStdToQ(d, "[ ");
            addStdToQ(d, ((PDDBManagedObject*)r.getItem())->getClassName());
            addStdToQ(d, " ]");
        }


        break;
    case GMCAction::Modify:
        {
        itemData << "Parameter" << "Mod";
        addStdToQ(d, "[ ");
            if ( ((PDDBManagedObjectParameter*)r.getItem())->getMocParent() != NULL )
            {
                PDDBManagedObject* mocObj = (PDDBManagedObject*)((PDDBManagedObjectParameter*)r.getItem())->getMocParent();
                addStdToQ(d, mocObj->getClassName());
                addStdToQ(d, "->");
            }
            addStdToQ(d, XmlElementReader::getAttributeByName(r.getItem()->getElement(), "name"));
            addStdToQ(d, "]");
        }
        break;
    }
    itemData << d;
    itemData << QString::fromStdString(r.getHelpNote());
}

