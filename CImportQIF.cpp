#include "CImportQIF.h"
#include <QtCore/QDate>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>

CImportQIF::CImportQIF()
{
}

CImportQIF::~CImportQIF()
{
}

bool CImportQIF::import(const QString& oFileName)
{
    QFile csv(oFileName);
    if (!csv.open(QFile::ReadOnly | QFile::Text))
        return false;

    QTextStream in(&csv);
    unsigned int uLineNo = 0;
    QString oLine;
    if (!in.atEnd())
    {
        ++uLineNo;
        oLine = in.readLine();
    }

    if (oLine == "!Type:Bank")
    {
        qDebug() << "Bank-type QIF";
        while (!in.atEnd())
        {
            uLineNo++;
            oLine = in.readLine();
        }
    }
    else if (oLine == "!Type:Invst")
    {
        qDebug() << "Investment-type QIF";
        while (!in.atEnd())
        {
            uLineNo++;
            oLine = in.readLine();
        }
    }
    else
    {
        qDebug() << "Unknown QIF file type";
        return false;
    }

    qDebug() << "Line count: " << uLineNo;
    return true;
}


