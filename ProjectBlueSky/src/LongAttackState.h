#pragma once
#include "AIState.h"

class LongAttackState :
	public AIState
{
public:

	static LongAttackState *GetInstance()
	{
		static LongAttackState s_Instance;
		return &s_Instance;
	}

private:

	LongAttackState();
	~LongAttackState();

	void Init(AICharacter* character);
	void Update(AICharacter* character);

	int shotCount;		// �e�̔��˃J�E���g
	int spEndCnt;		// �K�E�Z�̏I���J�E���g

	LongAttackState(const LongAttackState&);
	void operator=(const LongAttackState&) {}
};

