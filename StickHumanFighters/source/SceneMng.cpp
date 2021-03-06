#include "Dxlib.h"	// DxLibﾗｲﾌﾞﾗﾘを使用する
#include "FrameMng.h"
#include "GameCtrl.h"
#include "TitleScene.h"
#include "SceneMng.h"

#define SCREEN_SIZE_X (1280)
#define SCREEN_SIZE_Y (720)

void SceneMng::SetAILevel(int setLevel)
{
	AILevel = setLevel;
}

SceneMng::SceneMng()
{
	SysInit();
}

SceneMng::~SceneMng()
{
}


void SceneMng::Run(void)
{
	activeScene = std::make_unique<TitleScene>();

	// ---------- ｹﾞｰﾑﾙｰﾌﾟ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		frame->FrameStart();
		gameCtl->UpDate();

		ClsDrawScreen();

		activeScene = activeScene->UpDate(std::move(activeScene), *gameCtl);
		frame->FrameEnd();

		//frame->Draw();

		ScreenFlip();
	}
}

VECTOR2 SceneMng::GetScreenSize(void)
{
	return VECTOR2(SCREEN_SIZE_X, SCREEN_SIZE_Y);
}

const int SceneMng::GetCharID(PAD_ID padID)
{
	int charIdIndex;
	if (padID == PAD_1)
	{
		charIdIndex = 0;
	}
	else
	{
		charIdIndex = 1;
	}
	return charID[charIdIndex];
}

void SceneMng::SetCharID(PAD_ID padID,int id)
{
	int charIdIndex;
	if (padID == PAD_1)
	{
		charIdIndex = 0;
	}
	else
	{
		charIdIndex = 1;
	}
	charID[charIdIndex] = id;
}

bool SceneMng::GetSceneBackFlag(void)
{
	return sceneBackFlag;
}

void SceneMng::SetSceneBackFlag(bool sceneBackFlag)
{
	this->sceneBackFlag = sceneBackFlag;
}

bool SceneMng::GetTitleChangeFlag(void)
{
	return titleChangeFlag;
}

void SceneMng::SetTitleChangeFlag(bool titleChangeFlag)
{
	this->titleChangeFlag = titleChangeFlag;
}

PAD_ID SceneMng::GetOpenBackWindowPadID(void)
{
	return openBackWindowPadID;
}

void SceneMng::SetOpenBackWindowPadID(PAD_ID openBackWindowPadID)
{
	this->openBackWindowPadID = openBackWindowPadID;
}

bool SceneMng::GetDecidFlag(PAD_ID padID)
{
	return decidFlag[static_cast<int>(padID)];
}

void SceneMng::SetDecidFlag(PAD_ID padID, bool decidFlag)
{
	SceneMng::decidFlag[static_cast<int>(padID)] = decidFlag;
}

MODE SceneMng::GetMode(void)
{
	return mode;
}

void SceneMng::SetMode(MODE setMode)
{
	mode = setMode;
}

bool SceneMng::SysInit(void)
{
	// ｼｽﾃﾑ処理
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);		// 65536色ﾓｰﾄﾞに設定
	ChangeWindowMode(true);				// true:window　false:ﾌﾙｽｸﾘｰﾝ
	SetWindowText("StickHumanFighters");
	if (DxLib_Init() == -1) return false;	// DXﾗｲﾌﾞﾗﾘ初期化処理
	SetDrawScreen(DX_SCREEN_BACK);		// ひとまずﾊﾞｯｸﾊﾞｯﾌｧに描画
	gameCtl = std::make_unique<GameCtrl>();
	frame = std::make_unique<FrameMng>();

	for (int cnt = 0; cnt < PLAYER_CNT_MAX; cnt++)
	{
		charID[cnt] = -1;
	}

	mode = MODE_1PLAYER;
	sceneBackFlag = false;
	titleChangeFlag = false;
	openBackWindowPadID = PAD_1;
	AILevel = 0;

	return true;
}
