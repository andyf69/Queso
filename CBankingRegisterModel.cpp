#include "CBankingRegisterModel.h"
#include <QtSql/QSqlQuery>

CBankingRegisterModel::CBankingRegisterModel(QObject* pParent)
	: QSqlQueryModel(pParent)
{
}

void CBankingRegisterModel::setAccount(const int iAccountId)
{
    QSqlQuery q;
    q.prepare("SELECT t.Id, t.Date, t.SequenceNum, t.Amount, p.Name, t.Memo, c.Name, t.CheckNumber, t.Reconciled, t.Balance "
        "FROM [BankingTransaction] AS t "
        "LEFT JOIN [Payee] AS p ON p.Id=t.PayeeId "
        "LEFT JOIN [Category] AS c ON c.Id=t.CategoryId "
        "WHERE t.AccountId=:id "
        "ORDER BY t.Date, t.SequenceNum");
    q.bindValue(":id", iAccountId);
    q.exec();

    setQuery(q);
    setHeaderData(static_cast<int>(Column::Id), Qt::Horizontal, "Id");
    setHeaderData(static_cast<int>(Column::Date), Qt::Horizontal, "Date");
    setHeaderData(static_cast<int>(Column::SequenceNum), Qt::Horizontal, "SequenceNum");
    setHeaderData(static_cast<int>(Column::Amount), Qt::Horizontal, "Amount");
    setHeaderData(static_cast<int>(Column::Payee), Qt::Horizontal, "Payee");
    setHeaderData(static_cast<int>(Column::Memo), Qt::Horizontal, "Memo");
    setHeaderData(static_cast<int>(Column::Category), Qt::Horizontal, "Category");
    setHeaderData(static_cast<int>(Column::CheckNum), Qt::Horizontal, "Check#");
    setHeaderData(static_cast<int>(Column::Reconciled), Qt::Horizontal, "R");
    setHeaderData(static_cast<int>(Column::Balance), Qt::Horizontal, "Balance");

    // Model does lazy loading. Force it to read everything now.
    while (canFetchMore())
        fetchMore();
}
