#pragma once

#include <QtCore/QString>

class CImportQIF
{
public:
	CImportQIF();
	~CImportQIF();

	bool import(const QString& oFileName);

private:
};

