#pragma once
#include <QtCore/QString>

class CSettings
{
public:
	CSettings();
	~CSettings();

	QString importCSVPath() const;
	void setImportCSVPath(const QString& oPath);

	QString importQIFPath() const;
	void setImportQIFPath(const QString& oPath);
};

