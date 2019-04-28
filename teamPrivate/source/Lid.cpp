#include "DxLib.h"
#include "SceneMng.h"
#include "Collision.h"
#include "SoundMng.h"
#include "Lid.h"

#define SP_ATTACK_TIME_MAX	(900)
#define GUARD_TIME_MAX	(360)
#define FLY_HIGH_POWER	(VECTOR2(10,10))

Lid::Lid()
{
}

Lid::Lid(sharedListFloor floorList, VECTOR2 pos, int padNum)
{
	Obj::floorList		= floorList;
	Obj::pos			= pos;
	Obj::padNum			= padNum;

	Obj::init("双葉青砥");
}

Lid::~Lid()
{
}

const int Lid::GetPadNum(void)
{
	return Obj::padNum;
}

bool Lid::initAnim(void)
{
	// ｺﾗｲﾀﾞｰと画像のﾛｰﾄﾞ
	auto loadImageAndCol = [=](std::string animName, std::string dataName, int inv, bool loop, ANIM_TYPE animType)
	{
		lpImageMng.LoadContinueImageId(GetCharName() + "/" + animName, dataName);
		lpColMng.ColLoad(GetCharName(), animName);
		AddAnim(animName, inv, loop, dataName);
		animPtnTbl[animName] = animType;
	};

	//				ｱﾆﾒｰｼｮﾝ名			ﾃﾞｰﾀ名				ｺﾏ数	ﾙｰﾌﾟ	ｱﾆﾒｰｼｮﾝﾀｲﾌﾟ
	loadImageAndCol("ジャンプ始まり", "jumpStart", 6, false, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("ジャンプ中", "jump", 6, true, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("待機", "wait", 8, true, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("歩く", "walk", 6, true, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("走る", "dash", 6, true, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("ガード", "guard", 6, true, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("傘浮遊", "fly", 6, true, ANIM_TYPE_NO_ATTACK);
	loadImageAndCol("死亡", "death", 120, false, ANIM_TYPE_NO_ATTACK);

	loadImageAndCol("通常攻撃1", "attack", 4, false, ANIM_TYPE_MULTI_HIT);
	loadImageAndCol("通常攻撃2", "attackSecond", 4, false, ANIM_TYPE_MULTI_HIT);
	loadImageAndCol("通常攻撃3", "attackThird", 6, false, ANIM_TYPE_MULTI_HIT);

	loadImageAndCol("通常攻撃_上", "attackUp", 8, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("通常攻撃_下始まり", "attackDownStart", 6, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("通常攻撃_下中_空中", "attackDown", 6, true, ANIM_TYPE_ATTACK);
	loadImageAndCol("通常攻撃_下中_地上", "attackDown", 6, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("ダメージ", "damage", 45, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("必殺技", "specialAttack", 8, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("溜め攻撃", "chargeAttack", 3, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("溜め始まり", "chargeStart", 6, false, ANIM_TYPE_ATTACK);
	loadImageAndCol("溜め中", "charge", 6, true, ANIM_TYPE_ATTACK);
	loadImageAndCol("怒りゲージ溜め", "angryCharge", 1, true, ANIM_TYPE_ATTACK);
	loadImageAndCol("回避", "avoid", 10, false, ANIM_TYPE_ATTACK);

	SetAnim("待機");
	
	//SEの登録
	lpSoundMng.SoundInit("sound/SE類/Elefant.mp3", "ﾊﾟｵｰﾝ");

	spSeCnt = 0;

	return true;
}

void Lid::Draw(bool pauseFlag, CUT_IN_INFO cutInInfo)
{
	Obj::Draw(pauseFlag, cutInInfo);
	if (GetAnim() == "溜め攻撃")
	{
		int cnt = animCnt / 2;	// ++されるよー 
		DrawRectGraph(
			pos.x - 150 / 2 + (static_cast<int>(dir) * 2 - 1) * 40, pos.y - 150 / 2,
			150 * (cnt % 5), 150 * (cnt / 5 % 2),
			150, 150,
			lpImageMng.GetID("image/UI/衝撃24.png")[0], true, static_cast<int>(dir));
	}
}

const VECTOR2 Lid::GetFlyHighPower(void)
{
	return VECTOR2(FLY_HIGH_POWER.x * (static_cast<int>(dir) * -2 + 1), FLY_HIGH_POWER.y);
}

bool Lid::CheckObjType(OBJ_TYPE type)
{
	return (OBJ_TYPE_PLAYER == type);
}

void Lid::SetMove(const GameController & controller, weakListObj objList, weakListChicken chickenList, bool entryFlag)
{
}

void Lid::UniqueAttack(VECTOR2 & tmpPos)
{
	if (GetAnim() == "溜め攻撃")
	{
		tmpPos.x += speed * ((static_cast<int>(CheckCharge()) + 1) * 7 / CHARGE_MAX) * (static_cast<int>(dir) * -2 + 1) * ((angryFlag || spAttackCnt) + 1);
	}
}

int Lid::GetSpAttackTimeMax(void)
{
	return SP_ATTACK_TIME_MAX;
}

int Lid::GetGuardTimeMax(void)
{
	return GUARD_TIME_MAX;
}

void Lid::PlaySPSe(void)
{
	if (spSeCnt == 10)
	{
		lpSoundMng.SetSound("ﾊﾟｵｰﾝ");
	}
}
