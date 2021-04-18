#pragma once
#include <QtSql/QSqlQueryModel>

class CBankingRegisterModel : public QSqlQueryModel
{
public:
	enum class Column
	{
		Id = 0,
		Date,
		SequenceNum,
		Amount,
		Payee,
		Memo,
		Category,
		CheckNum,
		Reconciled,
		Balance
	};

public:
	explicit CBankingRegisterModel(QObject* pParent);

	void setAccount(const int iAccountId);
};

