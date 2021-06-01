#include "CSettings.h"
#include <QtCore/QSettings>

Queso::CSettings::CSettings()
{
}

Queso::CSettings::~CSettings()
{
}

QString Queso::CSettings::importCSVPath() const
{
    QSettings oSettings;
    return oSettings.value("FileBrowsers/ImportCSVPath").toString();
}

void Queso::CSettings::setImportCSVPath(const QString& oPath)
{
    QSettings oSettings;
    oSettings.setValue("FileBrowsers/ImportCSVPath", oPath);
}

QString Queso::CSettings::importQIFPath() const
{
    QSettings oSettings;
    return oSettings.value("FileBrowsers/ImportQIFPath").toString();
}

void Queso::CSettings::setImportQIFPath(const QString& oPath)
{
    QSettings oSettings;
    oSettings.setValue("FileBrowsers/ImportQIFPath", oPath);
}

QByteArray Queso::CSettings::mainWindowGeometry() const
{
    QSettings oSettings;
    return oSettings.value("App/MainWindow/Geometry").toByteArray();
}

void Queso::CSettings::setMainWindowGeometry(const QByteArray& oGeometry)
{
    QSettings oSettings;
    oSettings.setValue("App/MainWindow/Geometry", oGeometry);
}

QByteArray Queso::CSettings::mainWindowState() const
{
    QSettings oSettings;
    return oSettings.value("App/MainWindow/State").toByteArray();
}

void Queso::CSettings::setMainWindowState(const QByteArray& oState)
{
    QSettings oSettings;
    oSettings.setValue("App/MainWindow/State", oState);
}

QByteArray Queso::CSettings::mainWindowVerticalSplitterState() const
{
    QSettings oSettings;
    return oSettings.value("App/MainWindow/VerticalSplitter/State").toByteArray();
}

void Queso::CSettings::setMainWindowVerticalSplitterState(const QByteArray& oState)
{
    QSettings oSettings;
    oSettings.setValue("App/MainWindow/VerticalSplitter/State", oState);
}
