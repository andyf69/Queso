#pragma once
#include "CEntity.h"
#include <QtCore/QString>

class CFinancialInstitution : public CEntity
{
public:
	CFinancialInstitution() { m_iId = 0; }
	~CFinancialInstitution() = default;

	virtual bool dbCreate();
	virtual bool dbRead(int id);
	virtual bool dbUpdate();
	virtual bool dbDelete();

	int id() const { return m_iId; }
	const QString& name() const { return m_oName; }

private:
	CFinancialInstitution(const CFinancialInstitution&) = delete;
	CFinancialInstitution& operator=(const CFinancialInstitution&) = delete;

	int m_iId;
	QString m_oName;
};

