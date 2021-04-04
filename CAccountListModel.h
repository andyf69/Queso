#pragma once
#include <QtGui/QStandardItemModel>

class CAccountListModel : public QStandardItemModel
{
public:
	explicit CAccountListModel(QObject* pParent = nullptr);
	~CAccountListModel() = default;
	
	int accountId(const QModelIndex& oIndex) const;
	int accountTypeId(const QModelIndex& oIndex) const;
	QString accountName(const QModelIndex& oIndex) const;
	QString InstitutionName(const QModelIndex& oIndex) const;
};


