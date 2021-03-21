#include "CImportCSV.h"
#include <QtCore/QDate>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>

CImportCSV::CImportCSV()
{
}

CImportCSV::~CImportCSV()
{
}

static bool read_csv_row(QTextStream& in, std::vector<QString>* pRowData)
{
    // https://stackoverflow.com/a/40229435
    static const int delta[][5] = {
        //  ,    "   \n    ?  eof
        {   1,   2,  -1,   0,  -1  }, // 0: parsing (store char)
        {   1,   2,  -1,   0,  -1  }, // 1: parsing (store column)
        {   3,   4,   3,   3,  -2  }, // 2: quote entered (no-op)
        {   3,   4,   3,   3,  -2  }, // 3: parsing inside quotes (store char)
        {   1,   3,  -1,   0,  -1  }, // 4: quote exited (no-op)
        // -1: end of row, store column, success
        // -2: eof inside quotes
    };

    pRowData->clear();

    if (in.atEnd())
        return false;

    int state = 0, t;
    char ch;
    QString cell;

    while (state >= 0)
    {
        if (in.atEnd())
            t = 4;
        else
        {
            in >> ch;
            if (ch == ',') t = 0;
            else if (ch == '\"') t = 1;
            else if (ch == '\n') t = 2;
            else t = 3;
        }

        state = delta[state][t];

        switch (state)
        {
        case 0:
        case 3:
            cell += ch;
            break;
        case -1:
        case 1:
            pRowData->push_back(cell);
            cell = "";
            break;
        }
    }

    if (state == -2)
        return false; // throw runtime_error("End-of-file found while inside quotes.");

    return true;
}

bool CImportCSV::import(const QString& oFileName)
{
    QFile csv(oFileName);
    if (!csv.open(QFile::ReadOnly | QFile::Text))
        return false;

    QTextStream in(&csv);
    unsigned int uLineNo = 0;
    unsigned int uRowsImported = 0;
    std::vector<QString> row;
    while (read_csv_row(in, &row))
    {
        uLineNo++;
        if (importRow(row))
            ++uRowsImported;
    }

    qDebug() << "Imported transaction count: " << uRowsImported;
    return true;
}

bool CImportCSV::importRow(const std::vector<QString>& row)
{
    if (row.size() != 13)
        return false;

    // Transaction ID, Posting Date, Effective Date, Transaction Type, Amount, Check Number, Reference Number, Description, Transaction Category, Type, Balance, Memo, Extended Description
    const QString& oTransId = row[0];
    if (isTransactionImportedAlready(oTransId))
        return false;

    QDate oPostingDate(QDate::fromString(row[1], "M/d/yyyy"));
    if (!oPostingDate.isValid())
        return false;

    QDate oEffectiveDate(QDate::fromString(row[2], "M/d/yyyy"));
    if (!oEffectiveDate.isValid())
        return false;

    const QString& oTransType = row[3];
    const QString& oAmount = row[4];
    const QString& oCheckNo = row[5];
    const QString& oRefNo = row[6];
    const QString& oDescription = row[7];
    const QString& oTransCat = row[8];
    const QString& oType = row[9];
    const QString& oBalance = row[10];

    QStringList oTidBits = oTransId.split(' ');
    if (oTidBits.size() != 4)
        return false;

    QDate oTidDate(QDate::fromString(oTidBits[0], "yyyyMMdd"));
    const QString& oFIAcctNum = oTidBits[1];
    const QString& oCents = oTidBits[2];
    QString oDateSeq = oTidBits[3];
    oDateSeq.remove(',');
    short iSeqNum = 0;
    if (oDateSeq.size() == 12)
        iSeqNum = oDateSeq.right(4).toShort();

    const QString& oMemo = row[11];
    const QString& oExtendedDescription = row[12];

    //qDebug() << oCheckNo << oCheckNo.size() << oCheckNo.isEmpty() << oCheckNo.isNull();
    //qDebug() << oTidDate << " ACCT:" << iFIAcctNum << " Cents:" << oCents << " Seq:" << iSeqNum;
    //qDebug() /*<< "ID:" << oTransId*/ << " PD:" << oPostingDate << " ED:" << oEffectiveDate << " TT:" << oTransType << " AM:" << oAmount << " CN:" << oCheckNo
    //    << " RN:" << oRefNo << " DE:" << oDescription << " TC:" << oTransCat << " TY:" << oType << " BA:" << oBalance;

    QSqlQuery q;
    q.prepare("INSERT INTO FITransaction (TransactionId, PostingDate, EffectiveDate, SequenceNum, FIAccountNum, CheckNumber, ReferenceNumber, Description, TransactionCategory, Type, Amount, Balance, Memo, ExtendedDescription) "
        "VALUES(:transid, :pd, :ed, :seq, :fiacct, :chk, :ref, :desc, :cat, :type, :amt, :bal, :memo, :extdesc)");
    q.bindValue(":transid", oTransId);
    q.bindValue(":pd", oPostingDate.toString(Qt::ISODate));
    q.bindValue(":ed", oEffectiveDate.toString(Qt::ISODate));
    q.bindValue(":seq", iSeqNum);
    q.bindValue(":fiacct", oFIAcctNum.isEmpty() ? QVariant() : oFIAcctNum);
    q.bindValue(":chk", oCheckNo.isEmpty() ? QVariant() : oCheckNo);
    q.bindValue(":ref", oRefNo);
    q.bindValue(":desc", oDescription.isEmpty() ? QVariant() : oDescription);
    q.bindValue(":cat", oTransCat.isEmpty() ? QVariant() : oTransCat);
    q.bindValue(":type", oType.isEmpty() ? QVariant() : oType);
    q.bindValue(":amt", oAmount);
    q.bindValue(":bal", oBalance);
    q.bindValue(":memo", oMemo);
    q.bindValue(":extdesc", oExtendedDescription);

    if (q.exec())
        qDebug() << "Success";
    else
        qDebug() << "Error";

    return true;
}

bool CImportCSV::isTransactionImportedAlready(const QString& oTransactionID)
{
    QSqlQuery q;
    q.prepare("SELECT Id FROM FITransaction WHERE TransactionId = :transid");
    q.bindValue(":transid", oTransactionID);
    if (q.exec())
    {
        if (q.first())
        {
            //qDebug() << "Id: " << q.value(0).toInt();
            return true;
        }
    }

    return false;
}
