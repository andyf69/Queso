#include "CBankingTransaction.h"
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>

// SELECT [Id], [Date], [SequenceNum], [Amount], [PayeeId], [Memo], [AccountId], [CategoryId], [CheckNumber], [Reconciled], [Balance]
// FROM[Queso].[dbo].[BankingTransaction]
 
bool Queso::CBankingTransaction::dbCreate()
{
	QSqlQuery q;
	q.prepare("INSERT INTO BankingTransaction (Date, SequenceNum, Amount, PayeeId, Memo, AccountId, CategoryId, CheckNumber, Reconciled, Balance) "
		"VALUES(:date, :seqno, :amt, :payeeid, :memo, :acctid, :catid, :checkno, :recon, :bal)");
	q.bindValue(":date", m_oDate);
	q.bindValue(":seqno", m_iSequenceNum);
	q.bindValue(":amt", QString::fromStdString(m_oAmount.asString()));
	q.bindValue(":payeeid", m_iPayeeId);
	q.bindValue(":memo", m_oMemo);
	q.bindValue(":acctid", m_iAccountId);
	q.bindValue(":catid", m_iCategoryId);
	q.bindValue(":checkno", m_iCheckNumber);
	q.bindValue(":recon", m_cReconciled);
	q.bindValue(":bal", QString::fromStdString(m_oBalance.asString()));
	if (q.exec())
	{
		m_iId = q.lastInsertId().toInt();
		return true;
	}
	return false;
}

bool Queso::CBankingTransaction::dbRead(int id)
{
	QSqlQuery q;
	q.prepare("SELECT [Id], [Date], [SequenceNum], [Amount], [PayeeId], [Memo], [AccountId], [CategoryId], [CheckNumber], [Reconciled], [Balance] FROM [BankingTransaction] WHERE [Id]=:id");
	q.bindValue(":id", id);
	if (q.exec())
	{
		if (q.next())
		{
			m_iId = id;
			m_oDate = q.value("Date").toDate();
			m_iSequenceNum = q.value("SequenceNum").toInt();
			m_oAmount = dec::decimal2(q.value("Amount").toString().toStdString());
			m_iPayeeId = q.value("PayeeId").toInt();
			m_oMemo = q.value("Memo").toString();
			m_iAccountId = q.value("AccountId").toInt();
			m_iCategoryId = q.value("CategoryId").toInt();
			m_iCheckNumber = q.value("CheckNumber").toInt();
			m_cReconciled = q.value("Reconciled").toChar().toLatin1();
			m_oBalance = dec::decimal2(q.value("Balance").toString().toStdString());
			return true;
		}
	}

	return false;
}

bool Queso::CBankingTransaction::dbUpdate()
{
	QSqlQuery q;
	q.prepare("UPDATE BankingTransaction SET Date=:date"
		", SequenceNum=:seqno"
		", Amount=:amt"
		", PayeeId=:payeeid"
		", Memo=:memo"
		", AccountId=:acctid"
		", CategoryId=:catid"
		", CheckNumber=:checkno"
		", Reconciled=:recon"
		", Balance=:bal"
		" WHERE [Id]=:id");
	q.bindValue(":id", m_iId);
	q.bindValue(":date", m_oDate);
	q.bindValue(":seqno", m_iSequenceNum);
	q.bindValue(":amt", QString::fromStdString(m_oAmount.asString()));
	q.bindValue(":payeeid", m_iPayeeId);
	q.bindValue(":memo", m_oMemo);
	q.bindValue(":acctid", m_iAccountId);
	q.bindValue(":catid", m_iCategoryId);
	q.bindValue(":checkno", m_iCheckNumber);
	q.bindValue(":recon", m_cReconciled);
	q.bindValue(":bal", QString::fromStdString(m_oBalance.asString()));
	if (q.exec())
		return true;
	return false;
}

bool Queso::CBankingTransaction::dbDelete()
{
	QSqlQuery q;
	q.prepare("DELETE FROM BankingTransaction WHERE [Id]=:id");
	q.bindValue(":id", m_iId);
	if (q.exec())
		return true;
	return false;
}
