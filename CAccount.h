#pragma once
#include "CEntity.h"
#include "Decimal.h"
#include <QtCore/QString>

namespace Queso
{
	class CAccount : public CEntity
	{
	public:
		enum class Type
		{
			Undefined = 0,
			Banking,
			Investment,
			Retirement,
			Asset,
			Liability
		};

	public:
		static QString typeToTypeName(const Type eType);
		static QString typeToDisplayName(const Type eType);

	public:
		CAccount() { m_iId = m_iFIAccountId = m_iFinancialInstitutionId = 0; m_eAccountType = Type::Undefined;  m_bHidden = false; }
		~CAccount() = default;

		virtual bool dbCreate();
		virtual bool dbRead(int id);
		virtual bool dbUpdate();
		virtual bool dbDelete();

		int id() const { return m_iId; }
		Type accountType() const { return m_eAccountType; }
		int fiAccountId() const { return m_iFIAccountId; }
		int financialInstitutionId() const { return m_iFinancialInstitutionId; }
		const QString& name() const { return m_oName; }
		bool hidden() const { return m_bHidden; }
		const dec::decimal2& balance() const { return m_oBalance; }

	private:
		CAccount(const CAccount&) = delete;
		CAccount& operator=(const CAccount&) = delete;

		int m_iId;
		Type m_eAccountType;
		int m_iFIAccountId;
		int m_iFinancialInstitutionId;
		QString m_oName;
		dec::decimal2 m_oBalance;
		bool m_bHidden;
	};
}
