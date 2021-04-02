#pragma once
#include <QtCore/QByteArray>
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

	QByteArray mainWindowGeometry() const;
	void setMainWindowGeometry(const QByteArray& oGeometry);

	QByteArray mainWindowState() const;
	void setMainWindowState(const QByteArray& oState);

	QByteArray mainWindowVerticalSplitterState() const;
	void setMainWindowVerticalSplitterState(const QByteArray& oState);
};

