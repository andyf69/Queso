#include "CCategory.h"
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>


bool CCategory::dbCreate()
{
	QSqlQuery q;
	q.prepare("INSERT INTO Category (Name) VALUES(:name, :desc, :income)");
	q.bindValue(":name", m_oName);
	q.bindValue(":desc", m_oDescription);
	q.bindValue(":income", m_iIncome);
	if (q.exec())
	{
		m_iId = q.lastInsertId().toInt();
		return true;
	}
	return false;
}

bool CCategory::dbRead(int id)
{
	QSqlQuery q;
	q.prepare("SELECT [Id], [Name], [Description], [Income] FROM [Category] WHERE [Id]=:id");
	q.bindValue(":id", id);
	if (q.exec())
	{
		if (q.next())
		{
			m_iId = id;
			m_oName = q.value("Name").toString();
			m_oDescription = q.value("Description").toString();
			m_iIncome = q.value("Income").toUInt();
			return true;
		}
	}

	return false;
}

bool CCategory::dbUpdate()
{
	QSqlQuery q;
	q.prepare("UPDATE Category SET Name=':name' Description=':desc' Income=':income' WHERE Id=:id");
	q.bindValue(":id", m_iId);
	q.bindValue(":name", m_oName);
	q.bindValue(":desc", m_oDescription);
	q.bindValue(":income", m_iIncome);
	if (q.exec())
		return true;
	return false;
}

bool CCategory::dbDelete()
{
	QSqlQuery q;
	q.prepare("DELETE FROM Category WHERE Id=:id");
	q.bindValue(":id", m_iId);
	if (q.exec())
		return true;
	return false;
}
