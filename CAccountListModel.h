#pragma once
#include <QtSql/QSqlQueryModel>

class CAccountListModel : public QSqlQueryModel
{
public:
	explicit CAccountListModel(QObject* pParent = nullptr);
	~CAccountListModel() = default;

	int accountId(int iRow) const;
	int accountTypeId(int iRow) const;
	QString accountName(int iRow) const;
	QString InstitutionName(int iRow) const;
};

