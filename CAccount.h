#pragma once
#include "CEntity.h"
#include <QtCore/QString>

class CAccount : public CEntity
{
public:
	enum class Type
	{
		Banking = 1,
		Investment,
		Retirement,
		Asset,
		Liability
	};

public:
	static QString typeToTypeName(const Type eType);
	static QString typeToDisplayName(const Type eType);

public:
	CAccount() { m_iId = m_iAccountTypeId = m_iFIAccountId = m_iFinancialInstitutionId = 0; m_bHidden = false; }
	~CAccount() = default;

	virtual bool dbCreate();
	virtual bool dbRead(int id);
	virtual bool dbUpdate();
	virtual bool dbDelete();

	int id() const { return m_iId; }
	Type accountType() const { return static_cast<Type>(m_iAccountTypeId); }
	int fiAccountId() const { return m_iFIAccountId; }
	int financialInstitutionId() const { return m_iFinancialInstitutionId; }
	const QString& name() const { return m_oName; }
	bool hidden() const { return m_bHidden; }

private:
	CAccount(const CAccount&) = delete;
	CAccount& operator=(const CAccount&) = delete;

	int m_iId;
	int m_iAccountTypeId;
	int m_iFIAccountId;
	int m_iFinancialInstitutionId;
	QString m_oName;
	bool m_bHidden;
};

