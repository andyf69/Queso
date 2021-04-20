#pragma once
#include "CEntity.h"
#include <QtCore/QString>

class CPayee : public CEntity
{
public:
	CPayee() { m_iId = 0; }
	~CPayee() = default;

	virtual bool dbCreate();
	virtual bool dbRead(int id);
	virtual bool dbUpdate();
	virtual bool dbDelete();

	int id() const { return m_iId; }
	const QString& name() const { return m_oName; }
	void setName(const QString& oName) { m_oName = oName; }

private:
	CPayee(const CPayee&) = delete;
	CPayee& operator=(const CPayee&) = delete;

	int m_iId;
	QString m_oName;
};

