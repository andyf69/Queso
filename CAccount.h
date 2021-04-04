#pragma once
#include <QtCore/QString>

class CAccount
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

};

