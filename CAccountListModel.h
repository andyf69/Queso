#pragma once
#include <QtGui/QStandardItemModel>

class CAccountListModel : public QStandardItemModel
{
public:
	explicit CAccountListModel(bool bIncludeHidden, QObject* pParent = nullptr);
	~CAccountListModel() = default;
	
	int accountId(const QModelIndex& oIndex) const;
};

