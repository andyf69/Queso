#pragma once
#include "CEntity.h"
#include <QtCore/QString>

class CCategory : public CEntity
{
public:
	CCategory() { m_iId = 0; m_iIncome = 0; }
	~CCategory() = default;

	virtual bool dbCreate();
	virtual bool dbRead(int id);
	virtual bool dbUpdate();
	virtual bool dbDelete();

	int id() const { return m_iId; }
	const QString& name() const { return m_oName; }
	const QString& description() const { return m_oDescription; }
	unsigned char income() const { return m_iIncome; }

private:
	CCategory(const CCategory&) = delete;
	CCategory& operator=(const CCategory&) = delete;

	int m_iId;
	QString m_oName;
	QString m_oDescription;
	unsigned char m_iIncome;
};

