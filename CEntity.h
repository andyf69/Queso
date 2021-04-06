#pragma once
class CEntity
{
public:
	virtual ~CEntity() = default;

	virtual bool dbCreate() = 0;
	virtual bool dbRead(int id) = 0;
	virtual bool dbUpdate() = 0;
	virtual bool dbDelete() = 0;
};

