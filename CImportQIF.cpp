#include "CImportQIF.h"
#include <QtCore/QDate>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QRegularExpression>
#include <QtCore/QTextStream>
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>

namespace
{
    enum class QIFState
    {
        Undefined,
        BankAcct,       // !Type:Bank	    Bank account transactions
        CashAcct,       // !Type:Cash	    Cash account transactions
        CCardAcct,      // !Type:CCard	    Credit card account transactions
        InvestAcct,     // !Type:Invst	    Investment account transactions
        AssetAcct,      // !Type:Oth A	    Asset account transactions
        LiabilityAcct,  // !Type:Oth L	    Liability account transactions
        AccountList,    // !Account	        Account list or which account follows
        CategoryList,   // !Type:Cat	    Category list
        ClassList,      // !Type:Class	    Class list
        MemorizedList   // !Type:Memorized	Memorized transaction list
    };

    static QIFState lookup_state(const QString& oStr)
    {
        static std::map<QString, QIFState> m =
        {
            {"!Type:Bank", QIFState::BankAcct},
            {"!Type:Cash", QIFState::CashAcct},
            {"!Type:CCard", QIFState::CCardAcct},
            {"!Type:Invst", QIFState::InvestAcct},
            {"!Type:Oth A", QIFState::AssetAcct},
            {"!Type:Oth L", QIFState::LiabilityAcct},
            {"!Account", QIFState::AccountList},
            {"!Type:Cat", QIFState::CategoryList},
            {"!Type:Class", QIFState::ClassList},
            {"!Type:Memorized", QIFState::MemorizedList}
        };

        const auto& it = m.find(oStr);
        if (it != m.end())
            return it->second;
        return QIFState::Undefined;
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
}

CImportQIF::CImportQIF()
{
}

CImportQIF::~CImportQIF()
{
}

bool CImportQIF::import(const QString& oFileName)
{
    QIFState eState = QIFState::Undefined;
    m_uLineNo = 0;

    QFile csv(oFileName);
    if (!csv.open(QFile::ReadOnly | QFile::Text))
        return false;

    QTextStream in(&csv);
    QString oLine;
    while (!in.atEnd())
    {
        ++m_uLineNo;
        oLine = in.readLine();

        if (oLine.startsWith('!'))
            eState = lookup_state(oLine);
        else
        {
            // Read another transaction for the current QIF state
            switch (eState)
            {
            case QIFState::BankAcct:
                readBankAccountLine(oLine);
                break;
            case QIFState::CashAcct:
                readCashAccountLine(oLine);
                break;
            case QIFState::CCardAcct:
                readCreditCardAccountLine(oLine);
                break;
            case QIFState::InvestAcct:
                readInvestmentAccountLine(oLine);
                break;
            case QIFState::AssetAcct:
                readOtherAssetAccountLine(oLine);
                break;
            case QIFState::LiabilityAcct:
                readOtherLiabilityAccountLine(oLine);
                break;
            case QIFState::AccountList:
                readAccountListLine(oLine);
                break;
            case QIFState::CategoryList:
                readCategoryListLine(oLine);
                break;
            case QIFState::ClassList:
                readClassListLine(oLine);
                break;
            case QIFState::MemorizedList:
                readMemorizedListLine(oLine);
                break;
            default:
                break;
            }
        }
    }

    return true;
}

void CImportQIF::readBankAccountLine(const QString& oLine)
{
    static BankTransaction* s_pBankTransaction = nullptr;
    static SplitLine s_oSplit;

    if (!s_pBankTransaction)
        s_pBankTransaction = new BankTransaction;
    switch (oLine.toLatin1()[0])
    {
    case 'C':   // Cleared flag
        s_pBankTransaction->oClearedFlag = oLine.mid(1);
        break;
    case 'D':   // Date
        s_pBankTransaction->oDate = oLine.mid(1);
        break;
    case 'M': // Memo
        s_pBankTransaction->oMemo = oLine.mid(1);
        break;
    case 'N': // Check Number
        s_pBankTransaction->oNumber = oLine.mid(1);
        break;
    case 'P': // Payee
        s_pBankTransaction->oPayee = oLine.mid(1);
        break;
    case 'T': // Transaction amount
        s_pBankTransaction->oAmount = oLine.mid(1);
        break;
    case 'A':    // Address
        s_pBankTransaction->oAddressLines.push_back(oLine.mid(1));
        break;
    case 'L':    // Category or Transfer / Class
        s_pBankTransaction->oCategory = oLine.mid(1);
        break;
    case 'S':    // Category / class in split
        s_oSplit.oCategory = oLine.mid(1);
        break;
    case 'E':    // Memo in split
        s_oSplit.oMemo = oLine.mid(1);
        break;
    case '$':    // Dollar amount of split
        s_oSplit.oAmount = oLine.mid(1);
        s_pBankTransaction->oSplitLines.push_back(s_oSplit);
        s_oSplit = SplitLine();
        break;
    case '^': // End of record
        m_oBankTransactions.push_back(s_pBankTransaction);
        s_pBankTransaction = nullptr;
        break;
    default:
        qDebug() << m_uLineNo << " Unknown record in BankAcct: " << oLine;
        break;
    }
}

void CImportQIF::readCashAccountLine(const QString& oLine)
{
    static CashTransaction* s_pCashTransaction = nullptr;
    static SplitLine s_oSplit;

    if (!s_pCashTransaction)
        s_pCashTransaction = new CashTransaction;
    switch (oLine.toLatin1()[0])
    {
    case 'C':   // Cleared flag
        s_pCashTransaction->oClearedFlag = oLine.mid(1);
        break;
    case 'D':   // Date
        s_pCashTransaction->oDate = oLine.mid(1);
        break;
    case 'M': // Memo
        s_pCashTransaction->oMemo = oLine.mid(1);
        break;
    case 'N': // Check Number
        s_pCashTransaction->oNumber = oLine.mid(1);
        break;
    case 'P': // Payee
        s_pCashTransaction->oPayee = oLine.mid(1);
        break;
    case 'T': // Transaction amount
        s_pCashTransaction->oAmount = oLine.mid(1);
        break;
    case 'A':    // Address
        s_pCashTransaction->oAddressLines.push_back(oLine.mid(1));
        break;
    case 'L':    // Category or Transfer / Class
        s_pCashTransaction->oCategory = oLine.mid(1);
        break;
    case 'S':    // Category / class in split
        s_oSplit.oCategory = oLine.mid(1);
        break;
    case 'E':    // Memo in split
        s_oSplit.oMemo = oLine.mid(1);
        break;
    case '$':    // Dollar amount of split
        s_oSplit.oAmount = oLine.mid(1);
        s_pCashTransaction->oSplitLines.push_back(s_oSplit);
        s_oSplit = SplitLine();
        break;
    case '^': // End of record
        m_oCashTransactions.push_back(s_pCashTransaction);
        s_pCashTransaction = nullptr;
        break;
    default:
        qDebug() << m_uLineNo << " Unknown record in CashAcct: " << oLine;
        break;
    }
}

void CImportQIF::readCreditCardAccountLine(const QString& oLine)
{
    static CCardTransaction* s_pCCardTransaction = nullptr;
    static SplitLine s_oSplit;

    if (!s_pCCardTransaction)
        s_pCCardTransaction = new CCardTransaction;
    switch (oLine.toLatin1()[0])
    {
    case 'C':   // Cleared flag
        s_pCCardTransaction->oClearedFlag = oLine.mid(1);
        break;
    case 'D':   // Date
        s_pCCardTransaction->oDate = oLine.mid(1);
        break;
    case 'M': // Memo
        s_pCCardTransaction->oMemo = oLine.mid(1);
        break;
    case 'N': // Check Number
        s_pCCardTransaction->oNumber = oLine.mid(1);
        break;
    case 'P': // Payee
        s_pCCardTransaction->oPayee = oLine.mid(1);
        break;
    case 'T': // Transaction amount
        s_pCCardTransaction->oAmount = oLine.mid(1);
        break;
    case 'U':    // Ammount - Same as 'T'?
        // Ignore
        break;
    case 'A':    // Address
        s_pCCardTransaction->oAddressLines.push_back(oLine.mid(1));
        break;
    case 'L':    // Category or Transfer / Class
        s_pCCardTransaction->oCategory = oLine.mid(1);
        break;
    case 'S':    // Category / class in split
        s_oSplit.oCategory = oLine.mid(1);
        break;
    case 'E':    // Memo in split
        s_oSplit.oMemo = oLine.mid(1);
        break;
    case '$':    // Dollar amount of split
        s_oSplit.oAmount = oLine.mid(1);
        s_pCCardTransaction->oSplitLines.push_back(s_oSplit);
        s_oSplit = SplitLine();
        break;
    case '^': // End of record
        m_oCCardTransactions.push_back(s_pCCardTransaction);
        s_pCCardTransaction = nullptr;
        break;
    default:
        qDebug() << m_uLineNo << " Unknown record in CCardAcct: " << oLine;
        break;
    }
}

void CImportQIF::readInvestmentAccountLine(const QString& oLine)
{
    static InvestmentTransaction* s_pInvestmentTransaction = nullptr;
    if (!s_pInvestmentTransaction)
        s_pInvestmentTransaction = new InvestmentTransaction;
    switch (oLine[0].toLatin1())
    {
    case 'C':   // Cleared flag
        s_pInvestmentTransaction->oClearedFlag = oLine.mid(1);
        break;
    case 'D':   // Date
        s_pInvestmentTransaction->oDate = oLine.mid(1);
        break;
    case 'N':   // Action
        s_pInvestmentTransaction->oAction = oLine.mid(1);
        break;
    case 'Y':   // Security
        s_pInvestmentTransaction->oSecurity = oLine.mid(1);
        break;
    case 'M': // Memo
        s_pInvestmentTransaction->oMemo = oLine.mid(1);
        break;
    case 'P': // Payee
        s_pInvestmentTransaction->oPayee = oLine.mid(1);
        break;
    case 'T': // Transaction amount
        s_pInvestmentTransaction->oAmount = oLine.mid(1);
        break;
    case 'U':    // Ammount - Same as 'T'?
        // Ignore
        break;
    case 'I':   // Price
        s_pInvestmentTransaction->oPrice = oLine.mid(1);
        break;
    case 'Q':   // Quantity
        s_pInvestmentTransaction->oQuantity = oLine.mid(1);
        break;
    case 'L': // Transfer account
        s_pInvestmentTransaction->oTransferAccount = oLine.mid(1);
        break;
    case '$': // Transfer amount
        s_pInvestmentTransaction->oTransferAmount = oLine.mid(1);
        break;
    case '^':   // End of entry
        m_oInvestmentTransactions.push_back(s_pInvestmentTransaction);
        s_pInvestmentTransaction = nullptr;
        break;
    default:
        qDebug() << m_uLineNo << " Unknown record in InvestmentAcct: " << oLine;
        break;
    }
}

void CImportQIF::readOtherAssetAccountLine(const QString & oLine)
{
    static OtherAssetTransaction* s_pOtherAssetTransaction = nullptr;
    if (!s_pOtherAssetTransaction)
        s_pOtherAssetTransaction = new OtherAssetTransaction;
    switch (oLine[0].toLatin1())
    {
    case 'C':   // Cleared flag
        s_pOtherAssetTransaction->oClearedFlag = oLine.mid(1);
        break;
    case 'D':   // Date
        s_pOtherAssetTransaction->oDate = oLine.mid(1);
        break;
    case 'M': // Memo
        s_pOtherAssetTransaction->oMemo = oLine.mid(1);
        break;
    case 'P': // Payee
        s_pOtherAssetTransaction->oPayee = oLine.mid(1);
        break;
    case 'T': // Transaction amount
        s_pOtherAssetTransaction->oAmount = oLine.mid(1);
        break;
    case 'U':    // Ammount - Same as 'T'?
        // Ignore
        break;
    case 'L':    // Category or Transfer / Class
        s_pOtherAssetTransaction->oCategory = oLine.mid(1);
        break;
    case '^':   // End of entry
        m_oOtherAssetTransactions.push_back(s_pOtherAssetTransaction);
        s_pOtherAssetTransaction = nullptr;
        break;
    default:
        qDebug() << m_uLineNo << " Unknown record in OtherAsset: " << oLine;
        break;
    }
}

void CImportQIF::readOtherLiabilityAccountLine(const QString & oLine)
{
    static OtherLiabilityTransaction* s_pOtherLiabilityTransaction = nullptr;
    static SplitLine s_oSplit;

    if (!s_pOtherLiabilityTransaction)
        s_pOtherLiabilityTransaction = new OtherLiabilityTransaction;
    switch (oLine[0].toLatin1())
    {
    case 'C':   // Cleared flag
        s_pOtherLiabilityTransaction->oClearedFlag = oLine.mid(1);
        break;
    case 'D':   // Date
        s_pOtherLiabilityTransaction->oDate = oLine.mid(1);
        break;
    case 'M': // Memo
        s_pOtherLiabilityTransaction->oMemo = oLine.mid(1);
        break;
    case 'P': // Payee
        s_pOtherLiabilityTransaction->oPayee = oLine.mid(1);
        break;
    case 'T': // Transaction amount
        s_pOtherLiabilityTransaction->oAmount = oLine.mid(1);
        break;
    case 'U':    // Ammount - Same as 'T'?
        // Ignore
        break;
    case 'L':    // Category or Transfer / Class
        s_pOtherLiabilityTransaction->oCategory = oLine.mid(1);
        break;
    case 'S':    // Category / class in split
        s_oSplit.oCategory = oLine.mid(1);
        break;
    case 'E':    // Memo in split
        s_oSplit.oMemo = oLine.mid(1);
        break;
    case '$':    // Dollar amount of split
        s_oSplit.oAmount = oLine.mid(1);
        s_pOtherLiabilityTransaction->oSplitLines.push_back(s_oSplit);
        s_oSplit = SplitLine();
        break;
    case '^':   // End of entry
        m_oOtherLiabilityTransactions.push_back(s_pOtherLiabilityTransaction);
        s_pOtherLiabilityTransaction = nullptr;
        break;
    default:
        qDebug() << m_uLineNo << " Unknown record in OtherLiability: " << oLine;
        break;
    }
}

void CImportQIF::readAccountListLine(const QString & oLine)
{
    static Account* s_pAccount = nullptr;
    if (!s_pAccount)
        s_pAccount = new Account;
    switch (oLine[0].toLatin1())
    {
    case 'N':   // Name
        s_pAccount->oName = oLine.mid(1);
        break;
    case 'T':   // Type of account
        s_pAccount->oType = oLine.mid(1);
        break;
    case 'D':   // Description
        s_pAccount->oDescription = oLine.mid(1);
        break;
    case 'L':   // Credit limit (only for credit card accounts)
        s_pAccount->oCreditLimit = oLine.mid(1);
        break;
    case '/':   // Statement balance date
        s_pAccount->oBalanceDate = oLine.mid(1);
        break;
    case '$':   // Statement balance amount
        s_pAccount->oBalanceAmount = oLine.mid(1);
        break;
    case '^':   // End of entry
        m_oAccounts.push_back(s_pAccount);
        s_pAccount = nullptr;
        break;
    default:
        qDebug() << m_uLineNo << " Unknown record in AccountList: " << oLine;
        break;
    }
}

void CImportQIF::readCategoryListLine(const QString& oLine)
{
    static Category* s_pCategory = nullptr;
    if (!s_pCategory)
        s_pCategory = new Category;
    switch (oLine[0].toLatin1())
    {
    case 'N':   // Name
        s_pCategory->oName = oLine.mid(1);
        break;
    case 'D':   // Description
        s_pCategory->oDescription = oLine.mid(1);
        break;
    case 'T':   // Tax related
        s_pCategory->bIsTaxRelated = true;
        break;
    case 'I':   // Income category
        s_pCategory->bIsIncomeCategory = true;
        break;
    case 'E':   // Expense category (default)
        s_pCategory->bIsIncomeCategory = false;
        break;
    case 'B':   // Budget amount
        s_pCategory->oBudgetAmount = oLine.mid(1);
        break;
    case 'R':   // Tax schedule info
        s_pCategory->oTaxScheduleInfo = oLine.mid(1);
        break;
    case '^':   // End of entry
        m_oCategories.push_back(s_pCategory);
        s_pCategory = nullptr;
        break;
    default:
        qDebug() << m_uLineNo << " Unknown record in CategoryList: " << oLine;
        break;
    }
}

void CImportQIF::readClassListLine(const QString& oLine)
{
    static Class* s_pClass = nullptr;
    if (!s_pClass)
        s_pClass = new Class;
    switch (oLine[0].toLatin1())
    {
    case 'N':   // Name
        s_pClass->oName = oLine.mid(1);
        break;
    case 'D':   // Description
        s_pClass->oDescription = oLine.mid(1);
        break;
    case '^':   // End of entry
        m_oClasses.push_back(s_pClass);
        s_pClass = nullptr;
        break;
    default:
        qDebug() << m_uLineNo << " Unknown record in ClassList: " << oLine;
        break;
    }
}

void CImportQIF::readMemorizedListLine(const QString& oLine)
{
    static Memorized* s_pMemorized = nullptr;
    static SplitLine s_oSplit;

    if (!s_pMemorized)
        s_pMemorized = new Memorized;
    switch (oLine[0].toLatin1())
    {
    case 'K':    // Transaction type
        s_pMemorized->oTransactionType = oLine.mid(1);
        break;
    case 'T':    // Amount
        s_pMemorized->oAmount = oLine.mid(1);
        break;
    case 'U':    // Ammount - Same as 'T'?
        // Ignore
        break;
    case 'C':    // Cleared status
        s_pMemorized->oClearedStatus = oLine.mid(1);
        break;
    case 'P':    // Payee
        s_pMemorized->oPayee = oLine.mid(1);
        break;
    case 'M':    // Memo
        s_pMemorized->oMemo = oLine.mid(1);
        break;
    case 'A':    // Address
        s_pMemorized->oAddressLines.push_back(oLine.mid(1));
        break;
    case 'L':    // Category or Transfer / Class
        s_pMemorized->oCategory = oLine.mid(1);
        break;
    case 'S':    // Category / class in split
        s_oSplit.oCategory = oLine.mid(1);
        break;
    case 'E':    // Memo in split
        s_oSplit.oMemo = oLine.mid(1);
        break;
    case '$':    // Dollar amount of split
        s_oSplit.oAmount = oLine.mid(1);
        s_pMemorized->oSplitLines.push_back(s_oSplit);
        s_oSplit = SplitLine();
        break;
    case '1':    // Amortization : First payment date
        s_pMemorized->oAmortizationFirstPaymentDate = oLine.mid(1);
        break;
    case '2':    // Amortization : Total years for loan
        s_pMemorized->oAmortizationTotalYears = oLine.mid(1);
        break;
    case '3':    // Amortization : Number of payments already made
        s_pMemorized->oAmortizationNumPaymentsMade = oLine.mid(1);
        break;
    case '4':    // Amortization : Number of periods per year
        s_pMemorized->oAmortizationPeriodsPerYear = oLine.mid(1);
        break;
    case '5':    // Amortization : Interest rate
        s_pMemorized->oAmortizationInterestRate = oLine.mid(1);
        break;
    case '6':    // Amortization : Current loan balance
        s_pMemorized->oAmortizationCurrentBalance = oLine.mid(1);
        break;
    case '7':    // Amortization : Original loan amount
        s_pMemorized->oAmortizationOriginalAmount = oLine.mid(1);
        break;
    case '^':   // End of entry
        m_oMemorized.push_back(s_pMemorized);
        s_pMemorized = nullptr;
        break;
    case '%':   // Split percentage
        // Ignore
        break;
    default:
        qDebug() << m_uLineNo << " Unknown record in MemorizedList: " << oLine;
        break;
    }
}

