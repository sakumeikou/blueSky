#pragma once
#include "Obj.h"

#include <map>

class Character :
	public Obj
{
public:
	Character();
	~Character();

	bool CheckObjType(OBJ_TYPE type);

	void Draw(void);		// 描画処理

private:
	bool InitAnim(void);

	std::map<std::string, std::string> animFileName;		// ｱﾆﾒｰｼｮﾝの画像ﾌｧｲﾙ名 (ｷｰ: ｱﾆﾒｰｼｮﾝ名)

protected:
	bool Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos);		// 初期化	引数: ﾌｧｲﾙ名, 分割ｻｲｽﾞ, 分割数, 座標

	std::string characterName;
};

