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
	static SceneMng &GetInstance(void)			// ���̸׽���߲�����擾����A�����żݸ���݂̏�Ԃɂ��Ă���
	{
		static SceneMng s_Instance;
		return s_Instance;
	}
	void Run(void);
	VECTOR2 GetScreenSize(void);						// ��ذݻ��ނ��擾����

	const int GetCharID(PAD_ID padID);
	void SetCharID(PAD_ID padID, int id);

	bool GetSceneBackFlag(void);
	void SetSceneBackFlag(bool sceneBackFlag);

	bool GetTitleChangeFlag(void);
	void SetTitleChangeFlag(bool titleChangeFlag);

	PAD_ID GetOpenBackWindowPadID(void);
	void SetOpenBackWindowPadID(PAD_ID openBackWindowPadID);

	//decidFlag�̏����擾����(�����FdecidFlag���擾��������ڲ԰�̔ԍ�)
	bool GetDecidFlag(PAD_ID padID);
	//decidFlag�̏���Ă���(�����FdecidFlag��Ă�������ڲ԰�̔ԍ�, ��Ă�����flag)
	void SetDecidFlag(PAD_ID padID, bool decidFlag);

	MODE GetMode(void);
	void SetMode(MODE setMode);

private:
	SceneMng();
	~SceneMng();

	SceneMng(const SceneMng&);
	void operator=(const SceneMng&) {}

	int charID[PLAYER_CNT_MAX];								// ��ׂ�ID

	std::shared_ptr<GameCtrl> gameCtl;		// GameController�׽���߲���i�[�p�ϐ�

	unique_Base activeScene;	// �e��݂ɱ������邽�߂��ư��߲���

	std::unique_ptr<FrameMng> frame;

	std::array<bool,PLAYER_CNT_MAX> decidFlag;		// ��בI�������肵�Ă��邩�ǂ������׸ށ@true:���肵�Ă���, false:���肵�Ă͂��Ȃ�
	bool sceneBackFlag;								// �O�̼�݂ɖ߂�邩���׸�
	bool titleChangeFlag;							// ���ټ�݂ɖ߂��׸�
	MODE mode;										// �ް�Ӱ�ފi�[�p�ϐ�
	PAD_ID openBackWindowPadID;							// �m�F����޳���J�����߯�ނ�ID

	bool SysInit(void);			// ���ѓI�ȕϐ��̏��������s��
};
