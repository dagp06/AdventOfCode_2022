#pragma once

#include "Common/Challenge.h"

#include <stack>

using namespace std;

class CChallenge_05 : public CChallenge
{
private:

	struct MovingStep
	{
		MovingStep(int _move, int _from, int _to) : m_Move(_move), m_From(_from), m_To(_to) {}

		int m_Move = 0;
		int m_From = 0;
		int m_To = 0;
	};


	virtual EErrorCode SetUp_FirstPart() override;
	virtual EErrorCode Run_FirstPart() override;
	virtual EErrorCode CleanUp_FirstPart() override;

	virtual EErrorCode SetUp_SecondPart() override;
	virtual EErrorCode Run_SecondPart() override;
	virtual EErrorCode CleanUp_SecondPart() override;

	static string const sm_inputFilePath;

	vector<stack<char>> m_CrateStacks;
	vector<MovingStep> m_MovingSteps;
};
