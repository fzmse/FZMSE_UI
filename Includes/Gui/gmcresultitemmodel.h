#pragma once

#include <QAbstractItemModel>
#include "Includes/Gui/gmcresultitem.h"
#include "Includes/InternalTypes/gmcaction.h"
#include <QDebug>
#include <vector>
class gmcResultItemModel;
#include "appGui.h"

class gmcResultItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    gmcResultItemModel(appGUI *app, QObject *parent = 0);
    ~gmcResultItemModel();

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    gmcResultItem * getItemFromRow(int row);
    void setResultVector(std::vector<InternalTypes::GMCAction> gmcResultList);
    void changeIncludeInGMC(const QModelIndex &);
    void setRoot();
    void clean();
    void selectNode();
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

private:
    appGUI * app;
    gmcResultItem * rootItem;
    QList<InternalTypes::GMCAction> results;
    void setupModelData();
};
