#include "CAccount.h"
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>

QString CAccount::typeToTypeName(const Type eType)
{
	switch (eType)
	{
	case Type::Banking:
		return "Banking";
	case Type::Investment:
		return "Investment";
	case Type::Retirement:
		return "Retirement";
	case Type::Asset:
		return "Asset";
	case Type::Liability:
		return "Liability";
	default:
		break;
	}
	return QString();
}

QString CAccount::typeToDisplayName(const Type eType)
{
	switch (eType)
	{
	case Type::Banking:
		return "Banking";
	case Type::Investment:
		return "Investments";
	case Type::Retirement:
		return "Retirement";
	case Type::Asset:
		return "Assets";
	case Type::Liability:
		return "Liabilities";
	default:
		break;
	}
	return QString();
}

bool CAccount::dbCreate()
{
	QSqlQuery q;
	q.prepare("INSERT INTO Account (Name, FIAccountId, AccountTypeId, FinancialInstitutionId, Hidden, Balance) "
		"VALUES(:name, :fiaid, :atid, :fiid, :hidden, :bal)");
	q.bindValue(":name", m_oName);
	q.bindValue(":fiaid", m_iFIAccountId);
	q.bindValue(":atid", static_cast<int>(m_eAccountType));
	q.bindValue(":fiid", m_iFinancialInstitutionId);
	q.bindValue(":hidden", m_bHidden);
	q.bindValue(":bal", QString::fromStdString(m_oBalance.asString()));
	if (q.exec())
	{
		m_iId = q.lastInsertId().toInt();
		return true;
	}
	return false;
}

bool CAccount::dbRead(int id)
{
	QSqlQuery q;
	q.prepare("SELECT [Id]"
		", [AccountTypeId]"
		", [FIAccountId]"
		", [FinancialInstitutionId]"
		", [Name]"
		", [Hidden]"
		", [Balance]"
		"FROM [Account]"
		"WHERE [Id]=:id");
	q.bindValue(":id", id);
	if (q.exec())
	{
		if (q.next())
		{
			m_iId = id;
			m_eAccountType = static_cast<Type>(q.value("AccountTypeId").toInt());
			m_iFIAccountId = q.value("FIAccountId").toInt();
			m_iFinancialInstitutionId = q.value("FinancialInstitutionId").toInt();
			m_oName = q.value("Name").toString();
			m_bHidden = q.value("Hidden").toBool();
			m_oBalance = dec::decimal2(q.value("Balance").toString().toStdString());
			return true;
		}
	}

	return false;
}

bool CAccount::dbUpdate()
{
	QSqlQuery q;
	q.prepare("UPDATE Account SET Name=':name'"
		", FIAccountId=:fiaid"
		", AccountTypeId=:atid"
		", FinancialInstitutionId=:fiid"
		", Hidden=:hidden"
		"WHERE Id=:id");
	q.bindValue(":id", m_iId);
	q.bindValue(":name", m_oName);
	q.bindValue(":fiaid", m_iFIAccountId);
	q.bindValue(":atid", static_cast<int>(m_eAccountType));
	q.bindValue(":fiid", m_iFinancialInstitutionId);
	q.bindValue(":hidden", m_bHidden);
	if (q.exec())
		return true;
	return false;
}

bool CAccount::dbDelete()
{
	QSqlQuery q;
	q.prepare("DELETE FROM Account WHERE Id=:id");
	q.bindValue(":id", m_iId);
	if (q.exec())
		return true;
	return false;
}
