#include <QAbstractItemModel>
#include "Includes/Gui/resultItem.h"
#include "Includes/InternalTypes/PDDBManagedObjectCompareResult.h"
#include <vector>


class resultItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    resultItemModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;

    void setResultVector(std::vector<InternalTypes::PDDBManagedObjectCompareResult> resultList);
    void setRoot();
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

private:
    resultItem * rootItem;
    QList<InternalTypes::PDDBManagedObjectCompareResult> results;
    void setupModelData(const QStringList &lines, resultItem *parent);

};
