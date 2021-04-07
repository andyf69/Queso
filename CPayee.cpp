#include "CPayee.h"
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>


bool CPayee::dbCreate()
{
	QSqlQuery q;
	q.prepare("INSERT INTO Payee (Name) VALUES(:name)");
	q.bindValue(":name", m_oName);
	if (q.exec())
	{
		m_iId = q.lastInsertId().toInt();
		return true;
	}
	return false;
}

bool CPayee::dbRead(int id)
{
	QSqlQuery q;
	q.prepare("SELECT [Id], [Name] FROM [Payee] WHERE [Id]=:id");
	q.bindValue(":id", id);
	if (q.exec())
	{
		if (q.next())
		{
			m_iId = id;
			m_oName = q.value("Name").toString();
			return true;
		}
	}

	return false;
}

bool CPayee::dbUpdate()
{
	QSqlQuery q;
	q.prepare("UPDATE Payee SET Name=':name' WHERE Id=:id");
	q.bindValue(":id", m_iId);
	q.bindValue(":name", m_oName);
	if (q.exec())
		return true;
	return false;
}

bool CPayee::dbDelete()
{
	QSqlQuery q;
	q.prepare("DELETE FROM Payee WHERE Id=:id");
	q.bindValue(":id", m_iId);
	if (q.exec())
		return true;
	return false;
}
