#pragma once

#include "Obj.h"

class Shot :
	public Obj
{
public:
	Shot(VECTOR2 pos, DIR dir, PAD_ID id);
	~Shot();

	void SetMove(const GameCtrl &ctl, weekListObj objList);

	bool CheckDeth();
	bool CheckObjType(OBJ_TYPE type);

private:
	VECTOR2 ssize;
	int speed;

	bool deleteFlag;			// 削除フラグ

	PAD_ID padID;				// 発射したキャラクターのPADID
};

