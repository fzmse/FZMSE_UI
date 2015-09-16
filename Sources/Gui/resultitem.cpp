
#include "Includes/Gui/resultItem.h"
#include <QIcon>
using namespace InternalTypes;

resultItem::resultItem(const QList<QVariant> &data,
                           resultItem *parent)
{
    this->parentItem = parent;
    this->itemData = data;
}

resultItem::resultItem(PDDBManagedObjectCompareResult resultObj,
                       resultItem *parent)
{
    this->resultObj = resultObj;
    this->parentItem = parent;
    setData();
}

resultItem::~resultItem()
{

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

resultItem * resultItem::findItemById(int id)
{
    for (int i = 0; i < itemList.size(); i++)
        if ( itemList.value(i)->resultObj.getId() == id )
            return itemList.value(i);
    return NULL;
}


void resultItem::setParetn(resultItem *item)
{
    parentItem = item;
}

QString resultItem::getLocation()
{
    return itemData.value(2).toString();
}

int resultItem::row() const
{
    if (parentItem)
        return parentItem->itemList.indexOf(const_cast<resultItem*>(this));

    return 0;
}

void resultItem::setData()
{
    QString description;
    switch(resultObj.getOrigin())
    {
    case PDDBManagedObjectCompareResult::Added:
        if ( resultObj.getScope() == PDDBManagedObjectCompareResult::ManagedObjectParameter )
        {
            itemData << "Parameter" << "Add";
            description.append("[ ");
            if ( ((PDDBManagedObjectParameter*)resultObj.getSecondElement())->getMocParent() != NULL )
            {
                description.append(QString::fromStdString(((PDDBManagedObject*)((PDDBManagedObjectParameter*)resultObj.getSecondElement())->getMocParent())->getClassName()));
                description.append(" -> ");
            }
            description.append(QString::fromStdString(XmlElementReader::getAttributeByName(resultObj.getSecondElement()->getElement(), "name")));
            description.append(" ]");
            itemData << description;
        }
        else if ( resultObj.getScope() == PDDBManagedObjectCompareResult::ManagedObject )
        {
            itemData << "Managed Object" << "Add";
            description.append("[ ");
            description.append(QString::fromStdString(((PDDBManagedObject*)resultObj.getSecondElement())->getClassName()));
            description.append(" ]");
            itemData << description;

        }
        else if ( resultObj.getScope() == PDDBManagedObjectCompareResult::ManagedObjectParameterComplexTypeParameter )
        {
            itemData << "Complex Parameter" << "Add";
            description.append("[ ");
            if ( ((PDDBManagedObjectParameter*)resultObj.getSecondElement())->getMocParent() != NULL )
            {
                description.append(QString::fromStdString(((PDDBManagedObject*)((PDDBManagedObjectParameter*)resultObj.getSecondElement())->getMocParent())->getClassName()));
                description.append(" -> ");
            }
            description.append(QString::fromStdString(XmlElementReader::getAttributeByName(resultObj.getSecondElement()->getElement(), "name"))) ;
            description.append(" -> ");
            description.append(QString::fromStdString(XmlElementReader::getAttributeByName(resultObj.getSecondElementComplexTypeParameter()->getElement(), "name"))) ;
            description.append(" ]");
            itemData << description;
        }
        break;
    case PDDBManagedObjectCompareResult::Removed:
        if ( resultObj.getScope() == PDDBManagedObjectCompareResult::ManagedObjectParameter )
        {
            itemData << "Parameter" << "Deleted";
            description.append("[ ");
            if ( ((PDDBManagedObjectParameter*)resultObj.getFirstElement())->getMocParent() != NULL )
            {
                PDDBManagedObject* mocObj = (PDDBManagedObject*)((PDDBManagedObjectParameter*)resultObj.getFirstElement())->getMocParent();
                description.append(QString::fromStdString(mocObj->getClassName()));
                description.append(" -> ");
            }
            description.append(QString::fromStdString(XmlElementReader::getAttributeByName(resultObj.getFirstElement()->getElement(), "name")));
            description.append(" ]");
            itemData << description;
        }

        else if ( resultObj.getScope() == PDDBManagedObjectCompareResult::ManagedObject )
        {
            itemData << "Managed Object" << "Deleted";
            description.append("[ ");
            description.append(QString::fromStdString(((PDDBManagedObject*)resultObj.getFirstElement())->getClassName()));
            description.append(" ]");
            itemData << description;
        }

        else if ( resultObj.getScope() == PDDBManagedObjectCompareResult::ManagedObjectParameterComplexTypeParameter )
        {
            itemData << "Complex Parameter" << "Deleted";
            description.append("[ ");
            if ( ((PDDBManagedObjectParameter*)resultObj.getFirstElement())->getMocParent() != NULL )
            {
                PDDBManagedObject* mocObj = (PDDBManagedObject*)((PDDBManagedObjectParameter*)resultObj.getFirstElement())->getMocParent();
                description.append(QString::fromStdString(mocObj->getClassName()));
                description.append(" -> ");
            }
            description.append(QString::fromStdString(XmlElementReader::getAttributeByName(resultObj.getFirstElement()->getElement(), "name")));
            description.append(" -> ");
            description.append(QString::fromStdString(XmlElementReader::getAttributeByName(resultObj.getFirstElementComplexTypeParameter()->getElement(), "name")));
            description.append(" ]");
            itemData << description;
        }
        break;
    case PDDBManagedObjectCompareResult::Modified:
        {
            if( resultObj.getScope() == PDDBManagedObjectCompareResult::ManagedObjectParameterComplexTypeParameter )
            {
                itemData << "Complex Parameter" << "Modified";
                description.append("[ ");
            }
            else
            {
                itemData << "Parameter" << "Modified";
                description.append("[ ");
            }

            if ( ((PDDBManagedObjectParameter*)resultObj.getFirstElement())->getMocParent() != NULL )
            {
                description.append(QString::fromStdString(((PDDBManagedObject*)((PDDBManagedObjectParameter*)resultObj.getFirstElement())->getMocParent())->getClassName()));
                description.append(" -> ");
            }
            description.append(QString::fromStdString(XmlElementReader::getAttributeByName(resultObj.getFirstElement()->getElement(), "name")));
            if ( resultObj.getScope() == PDDBManagedObjectCompareResult::ManagedObjectParameterComplexTypeParameter )
            {
                description.append(" -> ");
                description.append(QString::fromStdString(XmlElementReader::getAttributeByName(resultObj.getFirstElementComplexTypeParameter()->getElement(), "name")));
            }
            description.append(" ]");
            itemData << description;
            QString specification;

            vector<PDDBManagedObjectCompareResult::ChangeInMocParameterElement> changes = resultObj.getChangesInParameter();
            for ( vector<PDDBManagedObjectCompareResult::ChangeInMocParameterElement>::iterator itParChange = changes.begin();
                  itParChange != changes.end(); itParChange ++)
            {
                switch (*itParChange)
                {
                case PDDBManagedObjectCompareResult::Description:
                    specification.append("Description, ");
                break;
                case PDDBManagedObjectCompareResult::SimpleTypeValue:
                    specification.append("SimpleType value changed, ");
                break;
                case PDDBManagedObjectCompareResult::EnumerationChanged:
                    specification.append("Value Enumeration changed, ");
                break;
                case PDDBManagedObjectCompareResult::SimpleTypeValue_RangeChanged:
                    specification.append("Range Changed, ");
                break;
                case PDDBManagedObjectCompareResult::VendorSpecific:
                    specification.append("Vendor Specific, ");
                break;
                case PDDBManagedObjectCompareResult::CreationPriority:
                    specification.append("Creation Priority, ");
                break;
                case PDDBManagedObjectCompareResult::MoMinOccurs:
                    specification.append("Mo MinOccurs, ");
                break;
                case PDDBManagedObjectCompareResult::RelatedParameters:
                    specification.append("Related Parameters, ");
                break;
                case PDDBManagedObjectCompareResult::SimpleToComplex:
                    specification.append("SimpleValue changed to ComplexValue, ");
                break;
                case PDDBManagedObjectCompareResult::ComplexToSimple:
                    specification.append("ComplexValue changed to SimpleValue, ");
                break;
                case PDDBManagedObjectCompareResult::ComplexTypeValue:
                    specification.append("ComplexType value changed");
                break;

                }
            }
            itemData << specification;
        }
        break;
    }
}
