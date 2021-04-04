#include "CAccountListModel.h"
#include <QtSql/QSqlQuery>

namespace
{
    enum class Role { AccountId = Qt::UserRole + 1, AccountName, InstitutionName, AccountTypeId };
}

CAccountListModel::CAccountListModel(QObject* pParent)
    :QStandardItemModel(pParent)
{
    clear();

    QSqlQuery q;
    q.prepare("SELECT [Account].[Id]"
              ", [Account].[Name]"
              ", [FinancialInstitution].[Name] AS 'Institution'"
              ", [AccountType].[DisplayName]"
              "FROM [Account]"
              "LEFT JOIN [FinancialInstitution] ON [Account].[FinancialInstitutionId] = [FinancialInstitution].[Id]"
              "LEFT JOIN [AccountType] ON [Account].[AccountTypeId] = [AccountType].[Id]"
              "WHERE [Hidden] = 0"
              "ORDER BY [Account].[AccountTypeId], [FinancialInstitution].[Name], [Account].[Name]");
    if (q.exec())
    {
        setColumnCount(2);
        QStandardItem* pRootItem = invisibleRootItem();
        QStandardItem* pGroupItem = nullptr;
        while (q.next())
        {
            QString oGroup = q.value("DisplayName").toString();
            if (!pGroupItem || oGroup != pGroupItem->text())
            {
                pGroupItem = new QStandardItem(oGroup);
                pGroupItem->setEditable(false);
                pGroupItem->setFont(QFont("Calibri", 14));
                pGroupItem->setBackground(QColor(235, 235, 235));

                QStandardItem* pGroupValueItem = new QStandardItem("$543.21"); // TODO
                pGroupValueItem->setEditable(false);
                pGroupValueItem->setTextAlignment(Qt::AlignRight);
                pGroupValueItem->setFont(QFont("Calibri", 14));
                pGroupValueItem->setBackground(QColor(235, 235, 235));
                QList<QStandardItem*> oColumns;
                oColumns.push_back(pGroupItem);
                oColumns.push_back(pGroupValueItem);
                pRootItem->appendRow(oColumns);
            }

            QStandardItem* pNameItem = new QStandardItem(q.value("Name").toString());
            pNameItem->setEditable(false);
            pNameItem->setForeground(Qt::darkBlue);
            pNameItem->setData(q.value("Id"), static_cast<int>(Role::AccountId));
            pNameItem->setData(q.value("AccountTypeId"), static_cast<int>(Role::AccountTypeId));
            pNameItem->setData(q.value("Name"), static_cast<int>(Role::AccountName));
            pNameItem->setData(q.value("Institution"), static_cast<int>(Role::InstitutionName));

            QStandardItem* pValueItem = new QStandardItem("$12.34"); // TODO
            pValueItem->setEditable(false);
            pValueItem->setTextAlignment(Qt::AlignRight);

            QList<QStandardItem*> oColumns;
            oColumns.push_back(pNameItem);
            oColumns.push_back(pValueItem);

            pGroupItem->appendRow(oColumns);
        }
    }
}

int CAccountListModel::accountId(const QModelIndex& oIndex) const
{
    return oIndex.siblingAtColumn(0).data(static_cast<int>(Role::AccountId)).toInt();
}

int CAccountListModel::accountTypeId(const QModelIndex& oIndex) const
{
    return oIndex.siblingAtColumn(0).data(static_cast<int>(Role::AccountTypeId)).toInt();
}

QString CAccountListModel::accountName(const QModelIndex& oIndex) const
{
    return oIndex.siblingAtColumn(0).data(static_cast<int>(Role::AccountName)).toString();
}

QString CAccountListModel::InstitutionName(const QModelIndex& oIndex) const
{
    return oIndex.siblingAtColumn(0).data(static_cast<int>(Role::InstitutionName)).toString();
}
