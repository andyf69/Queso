#pragma once

#include <vector>
#include <QtCore/QString>

namespace Queso
{
	class CImportCSV
	{
	public:
		CImportCSV();
		~CImportCSV();

		bool import(const QString& oFileName);

	private:
		bool importRow(const std::vector<QString>& row);
		bool isTransactionImportedAlready(const QString& oTransID);
	};
}
