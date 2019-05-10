#include "Dxlib.h"
#include "SceneMng.h"
#include "ImageMng.h"
#include "Shot.h"
#include "Character.h"

#define DEF_COM_CLEAR_CNT (60)
#define JUMP_SPEED_X (4)
#define JUMP_SPEED_Y (30)

Character::Character(VECTOR2 offset) : Obj(offset)
{
}

Character::~Character()
{
}

bool Character::Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos, bool turn, PAD_ID id)
{
	Obj::Init(fileName, divSize, divCut, pos, turn);
	InitAnim();

	jumpSpeed = { 0, 0 };
	jumpFlag = false;
	padID = id;

	// 通常のアクション
	animFileName["待機"] = "stand";
	animFileName["前移動"] = "move_front";
	animFileName["後ろ移動"] = "move_back";
	animFileName["ジャンプ_上"] = "jump";
	animFileName["ジャンプ_前"] = "jump_front";
	animFileName["ジャンプ_後ろ"] = "jump_back";
	animFileName["しゃがみ"] = "squat";
	animFileName["しゃがみ_後ろ"] = "squat_back";
	animFileName["しゃがみ始め"] = "squat_start";
	animFileName["立ち始め"] = "stand_start";
	animFileName["パンチ_小"] = "punch_small";
	animFileName["パンチ_大"] = "punch_big";
	animFileName["ガード_しゃがみ"] = "guard_squat";

	std::vector<std::string> animName = { "待機",
										  "前移動",
										  "後ろ移動",
										  "ジャンプ_上",
										  "ジャンプ_前",
										  "ジャンプ_後ろ",
										  "しゃがみ",
										  "しゃがみ_後ろ",
										  "しゃがみ始め",
										  "立ち始め",
										  "パンチ_小", 
										  "パンチ_大",
										  "ガード_しゃがみ" };		// ｱﾆﾒｰｼｮﾝ名を要素として持つvector

	// 必殺技系
	if ((spAttackAnimName[0] != "技1") && (spAttackAnimFileName[0] != "waza_1"))
	{
		animFileName[spAttackAnimName[0]] = spAttackAnimFileName[0];
		animName.push_back(spAttackAnimName[0]);
	}
	if ((spAttackAnimName[1] != "技2") && (spAttackAnimFileName[1] != "waza_2"))
	{
		animFileName[spAttackAnimName[1]] = spAttackAnimFileName[1];
		animName.push_back(spAttackAnimName[1]);
	}
	if ((spAttackAnimName[2] != "技3") && (spAttackAnimFileName[2] != "waza_3"))
	{
		animFileName[spAttackAnimName[2]] = spAttackAnimFileName[2];
		animName.push_back(spAttackAnimName[2]);
	}

	// 一括読み込み
	lpImageMng.LoadImageCharacterAll(characterName, animName, animFileName);

	// comListの初期化
	if (!comList.empty())
	{
		comList.clear();
	}

	comDir = COM_DIR_CENTER;
	comDirOld = COM_DIR_CENTER;
	comClearCnt = DEF_COM_CLEAR_CNT;

	return true;
}

bool Character::InitAnim(void)
{
	AddAnim("待機", 0, 0, 6, 5, true);
	AddAnim("前移動", 0, 0, 8, 5, true);
	AddAnim("後ろ移動", 0, 0, 8, 5, true);
	AddAnim("ジャンプ_上", 0, 0, 10, 6, true);
	AddAnim("ジャンプ_前", 0, 0, 6, 6, true);
	AddAnim("ジャンプ_後ろ", 0, 0, 6, 6, true);
	AddAnim("しゃがみ", 0, 0, 6, 5, true);
	AddAnim("しゃがみ_後ろ", 0, 0, 1, 5, true);
	AddAnim("しゃがみ始め", 0, 0, 3, 5, false);
	AddAnim("立ち始め", 0, 0, 3, 5, false);
	AddAnim("パンチ_小", 0, 0, 7, 5, false);
	AddAnim("パンチ_大", 0, 0, 11, 5, false);
	AddAnim("ガード_しゃがみ", 0, 0, 1, 5, true);
	SetAnim("待機");
	return true;
}

void Character::CommandUpDate(const GameCtrl & ctl)
{
	// 入力方向の情報を更新
	comDirOld = comDir;

	if (ctl.GetPadData(padID, THUMB_L_UP))
	{
		if (ctl.GetPadData(padID, THUMB_L_RIGHT))
		{
			comDir = COM_DIR_RIGHT_UP;
		}
		else if (ctl.GetPadData(padID, THUMB_L_LEFT))
		{
			comDir = COM_DIR_LEFT_UP;
		}
		else
		{
			comDir = COM_DIR_UP;
		}
	}
	else if (ctl.GetPadData(padID, THUMB_L_DOWN))
	{
		if (ctl.GetPadData(padID, THUMB_L_RIGHT))
		{
			comDir = COM_DIR_RIGHT_DOWN;
		}
		else if (ctl.GetPadData(padID, THUMB_L_LEFT))
		{
			comDir = COM_DIR_LEFT_DOWN;
		}
		else
		{
			comDir = COM_DIR_DOWN;
		}
	}
	else
	{
		if (ctl.GetPadData(padID, THUMB_L_RIGHT))
		{
			comDir = COM_DIR_RIGHT;
		}
		else if (ctl.GetPadData(padID, THUMB_L_LEFT))
		{
			comDir = COM_DIR_LEFT;
		}
		else
		{
			comDir = COM_DIR_CENTER;
		}
	}

	// リストに入れる
	if (comDir != COM_DIR_CENTER)
	{
		if (comDir != comDirOld)
		{
			comList.push_back(comDir);
			comClearCnt = DEF_COM_CLEAR_CNT;
		}
	}

	// 入力受付時間の処理
	comClearCnt--;
	if ((!comList.empty()) && (comClearCnt < 0))
	{
		comList.clear();
	}
}

bool Character::CheckCommand(int skillNum)
{
	if (comList.size() < spAttackCommand[skillNum][dir].size())
	{
		return false;
	}
	else
	{
		auto itr = comList.end();
		int comNum = 0;

		// コマンド数分だけ戻す
		for (unsigned int buckCnt = 0; buckCnt < spAttackCommand[skillNum][dir].size(); buckCnt++)
		{
			itr--;
		}

		// 順番に比較
		while (itr != comList.end())
		{
			if ((*itr) != spAttackCommand[skillNum][dir][comNum])
			{
				break;
			}
			else
			{
				itr++;
				comNum++;

				// コマンド数分ループを抜けなかったら成功
				if (comNum == spAttackCommand[skillNum][dir].size())
				{
					comList.clear();
					return true;
				}
			}
		}
	}

	return false;
}

bool Character::CheckObjType(OBJ_TYPE type)
{
	return (type == OBJ_TYPE_CHARACTER);
}

void Character::SetMove(const GameCtrl & ctl, weekListObj objList)
{
	auto ssize = lpSceneMng.GetScreenSize();

	CommandUpDate(ctl);

	// キャラクター操作
	if (!jumpFlag)
	{
		dir = tmpDir;

		if (animTable[GetAnim()][ANIM_TBL_LOOP] || animEndFlag)
		{
			if (ctl.GetPadData(padID, THUMB_L_UP))		// ジャンプ
			{
				if (ctl.GetPadData(padID, THUMB_L_RIGHT))
				{
					jumpSpeed = { JUMP_SPEED_X, -JUMP_SPEED_Y };
					jumpFlag = true;

					if (dir == DIR_LEFT)
					{
						SetAnim("ジャンプ_後ろ");
					}
					else
					{
						SetAnim("ジャンプ_前");
					}
				}
				else if (ctl.GetPadData(padID, THUMB_L_LEFT))
				{
					jumpSpeed = { -JUMP_SPEED_X, -JUMP_SPEED_Y };
					jumpFlag = true;

					if (dir == DIR_LEFT)
					{
						SetAnim("ジャンプ_前");
					}
					else
					{
						SetAnim("ジャンプ_後ろ");
					}
				}
				else
				{
					jumpSpeed = { 0, -JUMP_SPEED_Y };
					jumpFlag = true;
					SetAnim("ジャンプ_上");
				}
			}
			else if (ctl.GetPadData(padID, THUMB_L_DOWN))		// しゃがみ
			{
				if ((GetAnim() == "しゃがみ始め") || (GetAnim() == "しゃがみ") || ((GetAnim() == "しゃがみ_後ろ")))
				{
					if (ctl.GetPadData(padID, THUMB_L_RIGHT))
					{
						if (dir == DIR_LEFT)
						{
							SetAnim("しゃがみ_後ろ");
						}
						else
						{
							SetAnim("しゃがみ");
						}
					}
					else if (ctl.GetPadData(padID, THUMB_L_LEFT))
					{
						if (dir == DIR_LEFT)
						{
							SetAnim("しゃがみ");
						}
						else
						{
							SetAnim("しゃがみ_後ろ");
						}
					}
					else
					{
						SetAnim("しゃがみ");
					}
				}
				else
				{
					SetAnim("しゃがみ始め");
				}
			}
			else
			{
				// 移動
				if (ctl.GetPadData(padID, THUMB_L_RIGHT))
				{
					pos.x += 4;
					if (dir == DIR_LEFT)
					{
						SetAnim("後ろ移動");
					}
					else
					{
						SetAnim("前移動");
					}
				}
				else if (ctl.GetPadData(padID, THUMB_L_LEFT))
				{
					pos.x -= 4;
					if (dir == DIR_LEFT)
					{
						SetAnim("前移動");
					}
					else
					{
						SetAnim("後ろ移動");
					}

				}
				else
				{
					SetAnim("待機");
				}
			}
			
			// 攻撃
			if (ctl.GetPadDataTrg(padID, BUTTON_X))
			{
				AddObjList()(objList, std::make_unique<Shot>(pos, dir, padID));
				SetAnim(spAttackAnimName[0]);
			}
			else if (ctl.GetPadData(padID, BUTTON_A))
			{
				if (CheckCommand(0))
				{
					AddObjList()(objList, std::make_unique<Shot>(pos, dir, padID));
					SetAnim(spAttackAnimName[0]);
				}
				else if (CheckCommand(1))
				{
					SetAnim("投げ");
				}
				else if (CheckCommand(2))
				{
					SetAnim("昇竜");
				}
				else
				{
					SetAnim("パンチ_小");
				}
			}
			else if (ctl.GetPadData(padID, BUTTON_B))
			{
				if (CheckCommand(0))
				{
					AddObjList()(objList, std::make_unique<Shot>(pos, dir, padID));
					SetAnim(spAttackAnimName[0]);
				}
				else if (CheckCommand(1))
				{
					SetAnim("投げ");
				}
				else if (CheckCommand(2))
				{
					SetAnim("昇竜");
				}
				else
				{
					SetAnim("パンチ_大");
				}
			}
		}
	}
	else
	{
		// ジャンプ中
		jumpSpeed.y += 1;
		pos += jumpSpeed;
	}

	if (pos.y > ssize.y)
	{
		pos.y = ssize.y;
		jumpFlag = false;
	}
}

void Character::Draw(void)
{
	if (imageName.length() == 0)
	{
		return;
	}
	int id = 0;
	if (animTable.find(animName) != animTable.end())
	{
		id = animCnt / animTable[animName][ANIM_TBL_INV];
		if (animTable[animName][ANIM_TBL_LOOP] || id < animTable[animName][ANIM_TBL_FRAME])
		{
			id %= animTable[animName][ANIM_TBL_FRAME];
		}
		else
		{
			id = animTable[animName][ANIM_TBL_FRAME] - 1;
			animEndFlag = true;
		}
	}

	std::string nowAnim = GetAnim();
	imageName = ("image/" + characterName + "/" + nowAnim + "/" + animFileName[nowAnim] + "_" + std::to_string(id) + ".png");

	// DIRがRIGHTの場合画像を反転
	if (dir == DIR_RIGHT)
	{
		turnFlag = false;
	}
	else
	{
		turnFlag = true;
	}

	if (visible)
	{
		DrawRotaGraph(pos.x, pos.y - divSize.y / 2, 1.0, 0.0, IMAGE_ID(imageName)[0], true, turnFlag);
	}
	animCnt++;

	int i = 0;
	for (auto& data : comList)
	{
		switch (data)
		{
		case COM_DIR_UP:
			DrawString((i * 40) + 15, 200, "上", 0xffffff);
			break;
		case COM_DIR_RIGHT_UP:
			DrawString((i * 40) + 15, 200, "右上", 0xffffff);
			break;
		case COM_DIR_RIGHT:
			DrawString((i * 40) + 15, 200, "右", 0xffffff);
			break;
		case COM_DIR_RIGHT_DOWN:
			DrawString((i * 40) + 15, 200, "右下", 0xffffff);
			break;
		case COM_DIR_DOWN:
			DrawString((i * 40) + 15, 200, "下", 0xffffff);
			break;
		case COM_DIR_LEFT_DOWN:
			DrawString((i * 40) + 15, 200, "左下", 0xffffff);
			break;
		case COM_DIR_LEFT:
			DrawString((i * 40) + 15, 200, "左", 0xffffff);
			break;
		case COM_DIR_LEFT_UP:
			DrawString((i * 40) + 15, 200, "左上", 0xffffff);
			break;
		default:
			break;
		}

		i++;
	}
}
