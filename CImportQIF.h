#pragma once

#include <vector>
#include <QtCore/QString>

class CImportQIF
{
public:
	struct SplitLine
	{
		QString oAmount;
		QString oCategory;
		QString oMemo;
	};

	struct BankTransaction
	{
		QString oDate;
		QString oAmount;
		QString oClearedFlag;
		QString oNumber;
		QString oPayee;
		QString oMemo;
		QString oCategory;
		std::vector<QString> oAddressLines;
		std::vector<SplitLine> oSplitLines;
	};

	struct CashTransaction
	{
		QString oDate;
		QString oAmount;
		QString oClearedFlag;
		QString oNumber;
		QString oPayee;
		QString oMemo;
		QString oCategory;
		std::vector<QString> oAddressLines;
		std::vector<SplitLine> oSplitLines;
	};

	struct CCardTransaction
	{
		QString oDate;
		QString oAmount;
		QString oClearedFlag;
		QString oNumber;
		QString oPayee;
		QString oMemo;
		QString oCategory;
		std::vector<QString> oAddressLines;
		std::vector<SplitLine> oSplitLines;
	};

	struct InvestmentTransaction
	{
		QString oDate;
		QString oAmount;
		QString oClearedFlag;
		QString oAction;
		QString oSecurity;
		QString oPrice;
		QString oQuantity;
		QString oPayee;
		QString oMemo;
		QString oTransferAccount;
		QString oTransferAmount;
	};

	struct OtherAssetTransaction
	{
		QString oDate;
		QString oAmount;
		QString oClearedFlag;
		QString oPayee;
		QString oMemo;
		QString oCategory;
	};

	struct OtherLiabilityTransaction
	{
		QString oDate;
		QString oAmount;
		QString oClearedFlag;
		QString oPayee;
		QString oMemo;
		QString oCategory;
		std::vector<SplitLine> oSplitLines;
	};

	struct Account
	{
		QString oName;
		QString oDescription;
		QString oType;
		QString oCreditLimit;
		QString oBalanceDate;
		QString oBalanceAmount;
	};

	struct Category
	{
		QString oName;
		QString oDescription;
		QString oBudgetAmount;
		QString oTaxScheduleInfo;
		bool bIsTaxRelated = false;
		bool bIsIncomeCategory = false;
	};

	struct Class
	{
		QString oName;
		QString oDescription;
	};

	struct Memorized
	{
		QString oTransactionType;
		QString oAmount;
		QString oClearedStatus;
		QString oPayee;
		QString oMemo;
		QString oCategory;
		QString oAmortizationFirstPaymentDate;
		QString oAmortizationTotalYears;
		QString oAmortizationNumPaymentsMade;
		QString oAmortizationPeriodsPerYear;
		QString oAmortizationInterestRate;
		QString oAmortizationCurrentBalance;
		QString oAmortizationOriginalAmount;
		std::vector<QString> oAddressLines;
		std::vector<SplitLine> oSplitLines;
	};

public:
	CImportQIF();
	~CImportQIF();

	bool import(const QString& oFileName);

	const std::vector<BankTransaction*> bankTransactions() { return m_oBankTransactions; }
	const std::vector<CashTransaction*> cashTransactions() { return m_oCashTransactions; }
	const std::vector<CCardTransaction*> creditCardTransactions() { return m_oCCardTransactions; }
	const std::vector<InvestmentTransaction*> investmentTransactions() { return m_oInvestmentTransactions; }
	const std::vector<OtherAssetTransaction*> otherAssetTransactions() { return m_oOtherAssetTransactions; }
	const std::vector<OtherLiabilityTransaction*> otherLiabilityTransactions() { return m_oOtherLiabilityTransactions; }
	const std::vector<Account*> accounts() { return m_oAccounts; }
	const std::vector<Category*> categories() { return m_oCategories; }
	const std::vector<Class*> classes() { return m_oClasses; }
	const std::vector<Memorized*> memorized() { return m_oMemorized; }

private:
	void readBankAccountLine(const QString& oLine);
	void readCashAccountLine(const QString& oLine);
	void readCreditCardAccountLine(const QString& oLine);
	void readInvestmentAccountLine(const QString& oLine);
	void readOtherAssetAccountLine(const QString& oLine);
	void readOtherLiabilityAccountLine(const QString& oLine);
	void readAccountListLine(const QString& oLine);
	void readCategoryListLine(const QString& oLine);
	void readClassListLine(const QString& oLine);
	void readMemorizedListLine(const QString& oLine);

	unsigned int m_uLineNo;
	std::vector<BankTransaction*> m_oBankTransactions;
	std::vector<CashTransaction*> m_oCashTransactions;
	std::vector<CCardTransaction*> m_oCCardTransactions;
	std::vector<InvestmentTransaction*> m_oInvestmentTransactions;
	std::vector<OtherAssetTransaction*> m_oOtherAssetTransactions;
	std::vector<OtherLiabilityTransaction*> m_oOtherLiabilityTransactions;
	std::vector<Account*> m_oAccounts;
	std::vector<Category*> m_oCategories;
	std::vector<Class*> m_oClasses;
	std::vector<Memorized*> m_oMemorized;
};

