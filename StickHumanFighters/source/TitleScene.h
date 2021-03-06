#pragma once
#include "BaseScene.h"

#include <array>
#include "VECTOR2.h"
#include "SceneMng.h"

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();

	unique_Base UpDate(unique_Base own, const GameCtrl &controller);

private:
	int Init();
	void TitleDraw();

	VECTOR2 ssize;
	bool m_selFlag;
	MODE mode;

	int frame;						// ﾌﾚｰﾑ数 (ｼｰﾝ移行直前は5倍速にしている)

	bool sceneChangeFlag;			// ｼｰﾝを切り替えてもいいかのﾌﾗｸﾞ (true: 切り替えてもよい, false: 切り替えてはいけない)
	int sceneChangeTime;			// sceneChangeFlagがtrueになってからｼｰﾝ移行するまでの時間

	int cutInFlag;
	bool flashFlag;

	VECTOR2 redManPos;
	VECTOR2 blueManPos;

	std::array<VECTOR2, MODE_MAX> posTbl;
};

