#include "CSettings.h"
#include <QtCore/QSettings>

CSettings::CSettings()
{
}

CSettings::~CSettings()
{
}

QString CSettings::importCSVPath() const
{
    QSettings oSettings;
    return oSettings.value("FileBrowsers/ImportCSVPath").toString();
}

void CSettings::setImportCSVPath(const QString& oPath)
{
    QSettings oSettings;
    oSettings.setValue("FileBrowsers/ImportCSVPath", oPath);
}

QString CSettings::importQIFPath() const
{
    QSettings oSettings;
    return oSettings.value("FileBrowsers/ImportQIFPath").toString();
}

void CSettings::setImportQIFPath(const QString& oPath)
{
    QSettings oSettings;
    oSettings.setValue("FileBrowsers/ImportQIFPath", oPath);
}

QByteArray CSettings::mainWindowGeometry() const
{
    QSettings oSettings;
    return oSettings.value("App/MainWindow/Geometry").toByteArray();
}

void CSettings::setMainWindowGeometry(const QByteArray& oGeometry)
{
    QSettings oSettings;
    oSettings.setValue("App/MainWindow/Geometry", oGeometry);
}

QByteArray CSettings::mainWindowState() const
{
    QSettings oSettings;
    return oSettings.value("App/MainWindow/State").toByteArray();
}

void CSettings::setMainWindowState(const QByteArray& oState)
{
    QSettings oSettings;
    oSettings.setValue("App/MainWindow/State", oState);
}

QByteArray CSettings::mainWindowVerticalSplitterState() const
{
    QSettings oSettings;
    return oSettings.value("App/MainWindow/VerticalSplitter/State").toByteArray();
}

void CSettings::setMainWindowVerticalSplitterState(const QByteArray& oState)
{
    QSettings oSettings;
    oSettings.setValue("App/MainWindow/VerticalSplitter/State", oState);
}
