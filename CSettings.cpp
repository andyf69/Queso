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
