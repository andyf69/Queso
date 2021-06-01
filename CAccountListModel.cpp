#include "CAccountListModel.h"
#include "CAccount.h"
#include "Decimal.h"
#include <QtSql/QSqlQuery>

namespace
{
    enum class Role { AccountId = Qt::UserRole + 1 };
}

Queso::CAccountListModel::CAccountListModel(bool bIncludeHidden, QObject* pParent)
    :QStandardItemModel(pParent)
{
    clear();

    QSqlQuery q;
    q.prepare("SELECT [Id], [Hidden]"
        "FROM [Account]"
        "ORDER BY [AccountTypeId], [Id]");
    if (q.exec())
    {
        setColumnCount(2);
        QStandardItem* pRootItem = invisibleRootItem();
        QStandardItem* pGroupItem = nullptr;
        while (q.next())
        {
            bool bHidden = q.value("Hidden").toBool();
            if (bHidden && !bIncludeHidden)
                continue;

            CAccount oAccount;
            oAccount.dbRead(q.value("Id").toInt());
            QString oGroup = CAccount::typeToDisplayName(oAccount.accountType());
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

            QStandardItem* pNameItem = new QStandardItem(oAccount.name());
            pNameItem->setEditable(false);
            if (bIncludeHidden)
            {
                //pNameItem->setCheckable(true);
                pNameItem->setCheckState(bHidden ? Qt::Checked : Qt:: Unchecked);
            }
            pNameItem->setForeground(Qt::darkBlue);
            pNameItem->setData(q.value("Id"), static_cast<int>(Role::AccountId));

            dec::decimal2 bal = oAccount.balance();
            QStandardItem* pValueItem = new QStandardItem(QString("$%1").arg(QString::fromStdString(bal.asString())));
            pValueItem->setEditable(false);
            pValueItem->setTextAlignment(Qt::AlignRight);

            QList<QStandardItem*> oColumns;
            oColumns.push_back(pNameItem);
            oColumns.push_back(pValueItem);

            pGroupItem->appendRow(oColumns);
        }
    }
}

int Queso::CAccountListModel::accountId(const QModelIndex& oIndex) const
{
    return oIndex.siblingAtColumn(0).data(static_cast<int>(Role::AccountId)).toInt();
}
