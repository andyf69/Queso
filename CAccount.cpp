#include "CAccount.h"

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
