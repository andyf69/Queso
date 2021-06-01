#pragma once
#include <QtGui/QStandardItemModel>

namespace Queso
{
	class CAccountListModel : public QStandardItemModel
	{
	public:
		explicit CAccountListModel(bool bIncludeHidden, QObject* pParent = nullptr);
		~CAccountListModel() = default;

		int accountId(const QModelIndex& oIndex) const;
	};
}
