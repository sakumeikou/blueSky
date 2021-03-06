#pragma once
#include <array>
#include <memory>
#include "VECTOR2.h"
#include "BaseScene.h"
#include "GameCtrl.h"
#define PLAYER_CNT_MAX (2)

class GameCtrl;
class FrameMng;

enum MODE {
	MODE_1PLAYER,
	MODE_2PLAYER,
	MODE_MAX
};

#define lpSceneMng SceneMng::GetInstance()

class SceneMng
{
public:
	static SceneMng &GetInstance(void)			// このｸﾗｽのﾎﾟｲﾝﾀを取得する、ここでｼﾝｸﾞﾙﾄﾝの状態にしている
	{
		static SceneMng s_Instance;
		return s_Instance;
	}
	void Run(void);
	VECTOR2 GetScreenSize(void);						// ｽｸﾘｰﾝｻｲｽﾞを取得する

	const int GetCharID(PAD_ID padID);
	void SetCharID(PAD_ID padID, int id);

	bool GetSceneBackFlag(void);
	void SetSceneBackFlag(bool sceneBackFlag);

	bool GetTitleChangeFlag(void);
	void SetTitleChangeFlag(bool titleChangeFlag);

	PAD_ID GetOpenBackWindowPadID(void);
	void SetOpenBackWindowPadID(PAD_ID openBackWindowPadID);

	//decidFlagの情報を取得する(引数：decidFlagを取得したいﾌﾟﾚｲﾔｰの番号)
	bool GetDecidFlag(PAD_ID padID);
	//decidFlagの情報をｾｯﾄする(引数：decidFlagをｾｯﾄしたいﾌﾟﾚｲﾔｰの番号, ｾｯﾄしたいflag)
	void SetDecidFlag(PAD_ID padID, bool decidFlag);

	MODE GetMode(void);
	void SetMode(MODE setMode);

	// AIのレベルを取得
	const int GetAILevel() const { return AILevel; }

	// AIのレベルのｾｯﾀｰ
	void SetAILevel(int setLevel);

private:
	SceneMng();
	~SceneMng();

	SceneMng(const SceneMng&);
	void operator=(const SceneMng&) {}

	int charID[PLAYER_CNT_MAX];								// ｷｬﾗのID

	std::shared_ptr<GameCtrl> gameCtl;		// GameControllerｸﾗｽのﾎﾟｲﾝﾀ格納用変数

	unique_Base activeScene;	// 各ｼｰﾝにｱｸｾｽするためのﾕﾆｰｸﾎﾟｲﾝﾀｰ

	std::unique_ptr<FrameMng> frame;

	std::array<bool,PLAYER_CNT_MAX> decidFlag;		// ｷｬﾗ選択を決定しているかどうかのﾌﾗｸﾞ　true:決定している, false:決定してはいない
	bool sceneBackFlag;								// 前のｼｰﾝに戻れるかのﾌﾗｸﾞ
	bool titleChangeFlag;							// ﾀｲﾄﾙｼｰﾝに戻るﾌﾗｸﾞ
	MODE mode;										// ｹﾞｰﾑﾓｰﾄﾞ格納用変数
	PAD_ID openBackWindowPadID;						// 確認ｳｨﾝﾄﾞｳを開いたﾊﾟｯﾄﾞのID

	int AILevel;									// AIの強さのレベル

	bool SysInit(void);			// ｼｽﾃﾑ的な変数の初期化を行う
};

