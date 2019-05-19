#pragma once
#include "BaseScene.h"

#include"ObjList.h"

class GameScene :
	public BaseScene
{
public:
	GameScene();
	~GameScene();

	unique_Base UpDate(unique_Base own, const GameCtrl &controller);		// 毎ﾌﾚｰﾑ処理するものをまとめた関数、state

private:
	int Init(void);
	int bgPos_x;
	VECTOR2 ssize;

	bool GameDraw(void);		// 描画処理
	int id[2];			// 当たり判定のフレーム数

	sharedListObj objList;
};

