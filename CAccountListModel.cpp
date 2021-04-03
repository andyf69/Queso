#include "CAccountListModel.h"

namespace
{
    enum class Column { AccountId, AccountName, InstitutionName, AccountTypeId };
}

CAccountListModel::CAccountListModel(QObject* pParent)
	:QSqlQueryModel(pParent)
{
    setQuery("SELECT [Account].[Id]"
             ", [Account].[Name]"
             ", [FinancialInstitution].[Name] AS 'Institution'"
             ", [Account].[AccountTypeId]"
             "FROM [Account]"
             "LEFT JOIN [FinancialInstitution] ON [Account].[FinancialInstitutionId] = [FinancialInstitution].[Id]"
             "WHERE [Hidden] = 0"
             "ORDER BY [Account].[AccountTypeId], [FinancialInstitution].[Name], [Account].[Name]");
}

int CAccountListModel::accountId(int iRow) const
{
    return data(index(iRow, static_cast<int>(Column::AccountId))).toInt();
}

int CAccountListModel::accountTypeId(int iRow) const
{
    return data(index(iRow, static_cast<int>(Column::AccountTypeId))).toInt();
}

QString CAccountListModel::accountName(int iRow) const
{
    return data(index(iRow, static_cast<int>(Column::AccountName))).toString();
}

QString CAccountListModel::InstitutionName(int iRow) const
{
    return data(index(iRow, static_cast<int>(Column::InstitutionName))).toString();
}
