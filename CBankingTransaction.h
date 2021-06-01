#pragma once
#include "CEntity.h"
#include "Decimal.h"
#include <QtCore/QDate>
#include <QtCore/QString>

namespace Queso
{
    class CBankingTransaction : public CEntity
    {
	public:
		CBankingTransaction()
		{
			m_iId = m_iAccountId = m_iCategoryId = m_iPayeeId = 0;
			m_iSequenceNum = 0;
			m_iCheckNumber = 0;
			m_cReconciled = ' ';
		}
		~CBankingTransaction() = default;

		virtual bool dbCreate();
		virtual bool dbRead(int id);
		virtual bool dbUpdate();
		virtual bool dbDelete();

		int id() const { return m_iId; }
		const QDate& date() const { return m_oDate; }
		int sequenceNum() const { return m_iSequenceNum; }
		const dec::decimal2& amount() const { return m_oAmount; }
		int payeeId() const { return m_iPayeeId; }
		const QString& memo() const { return m_oMemo; }
		int accountId() const { return m_iAccountId; }
		int categoryId() const { return m_iCategoryId; }
		int checkNumber() const { return m_iCheckNumber; }
		char reconciled() const { return m_cReconciled; }
		const dec::decimal2& balance() const { return m_oBalance; }

	private:
		CBankingTransaction(const CBankingTransaction&) = delete;
		CBankingTransaction& operator=(const CBankingTransaction&) = delete;

		int m_iId;
		int m_iAccountId;
		int m_iCategoryId;
		int m_iPayeeId;
		int m_iSequenceNum;
		int m_iCheckNumber;
		QDate m_oDate;
		QString m_oMemo;
		char m_cReconciled;
		dec::decimal2 m_oAmount;
		dec::decimal2 m_oBalance;
	};
}
