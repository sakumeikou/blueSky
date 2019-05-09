#include "CharSelCursor.h"
#include "SceneMng.h"
#include "DxLib.h"
#include "GameCtrl.h"

#define BOX_SIZE_X (100)		// ｷｬﾗｱｲｺﾝのX軸ｻｲｽﾞ
#define BOX_SIZE_Y (100)		// ｷｬﾗｱｲｺﾝのY軸ｻｲｽﾞ


CharSelCursor::CharSelCursor()
{
	Init();
}


CharSelCursor::~CharSelCursor()
{
}

void CharSelCursor::SetMove(const GameCtrl & ctl, weekListObj objList)
{
	/* ｶｰｿﾙの移動 */
	if (ctl.GetPadDataTrg(PAD_1, THUMB_L_UP))
	{
		pos.y -= BOX_SIZE_Y;
	}
	if (ctl.GetPadDataTrg(PAD_1, THUMB_L_RIGHT))
	{
		pos.x += BOX_SIZE_X;
	}
	if (ctl.GetPadDataTrg(PAD_1, THUMB_L_DOWN))
	{
		pos.y += BOX_SIZE_Y;
	}
	if (ctl.GetPadDataTrg(PAD_1, THUMB_L_LEFT))
	{
		pos.x -= BOX_SIZE_X;
	}

	/* ｶｰｿﾙの移動制御 */
	if (pos.x < (sSize.x / 2) - (BOX_SIZE_X * 2))
	{
		pos.x = (sSize.x / 2) - (BOX_SIZE_X * 2);
	}
	if (pos.x > (sSize.x / 2) + BOX_SIZE_X)
	{
		pos.x = (sSize.x / 2) + BOX_SIZE_X;
	}
	if (pos.y < (sSize.y * 3 / 5))
	{
		pos.y = (sSize.y * 3 / 5);
	}
	if (pos.y > (sSize.y * 3 / 5)+BOX_SIZE_Y)
	{
		pos.y = (sSize.y * 3 / 5) + BOX_SIZE_Y;
	}

}

bool CharSelCursor::CheckObjType(OBJ_TYPE type)
{
	return false;
}

int CharSelCursor::Init(void)
{
	sSize = lpSceneMng.GetScreenSize();
	pos.x = (sSize.x / 2) - (BOX_SIZE_X * 2);		// ｶｰｿﾙのX軸初期値
	pos.y = (sSize.y * 3 / 5);						// ｶｰｿﾙのY軸初期値
	return 0;
}

void CharSelCursor::Draw(void)
{
	
	DrawBox(pos.x, pos.y, pos.x + BOX_SIZE_X, pos.y+ BOX_SIZE_Y, 0xffffff, false);
}

