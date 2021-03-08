#include "CImportQIF.h"
#include <QtCore/QDate>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QRegularExpression>
#include <QtCore/QTextStream>
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>

CImportQIF::CImportQIF()
{
}

CImportQIF::~CImportQIF()
{
}

static QDate parse_date(const QString& oStr)
{
    QRegularExpression re("^D(\\d\\d?)/(\\d\\d?)'(\\d\\d\\d\\d)$");
    QRegularExpressionMatch m = re.match(oStr);
    if (m.hasMatch())
    {
        int day = m.captured(2).toInt();
        int month = m.captured(1).toInt();
        int year = m.captured(3).toInt();
        return QDate(year, month, day);
    }

    return QDate();
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
            if (oLine.isEmpty())
                continue;

            switch (oLine.toLatin1()[0])
            {
            case 'C': // Cleared flag
                break;
            case 'D': // Date
                //qDebug() << "Date: " << parse_date(oLine);
                break;
            case 'L': // Category
                //qDebug() << "Category: " << oLine.midRef(1);
                break;
            case 'M': // Memo
                //qDebug() << "Memo: " << oLine.midRef(1);
                break;
            case 'N': // Check Number
                //qDebug() << "Check#: " << oLine.midRef(1);
                break;
            case 'P': // Payee
                //qDebug() << "Payee: " << oLine.midRef(1);
                break;
            case 'T': // Transaction amount
                break;
            case 'E': // Memo in split
                break;
            case 'S': // Category in split
                break;
            case '$': // Amount in split
                break;
            case '^': // End of record
                break;
            default:
                qDebug() << "Line " << uLineNo << ": Unknown tag " << oLine.toLatin1()[0];
                break;
            }
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


