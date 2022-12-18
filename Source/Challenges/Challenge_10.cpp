#include "Challenge_10.h"
#include "../Helpers/FileHelpers.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_10::sm_inputFilePath = "Inputs/Input_Challenge_10.txt";


////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_10::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_OperationDuration[OperationType::noop] = 1;
    m_OperationDuration[OperationType::addx] = 2;

    for (const string& line : lines)
    {
        vector<string> operationStr;
        FileHelper::SplitLine(line, " ", operationStr);

        if (operationStr.size() == 1)
        {
            m_Operations.push_back(Operation(OperationType::noop));
        }
        else
        {
            m_Operations.push_back(Operation(OperationType::addx, stoi(operationStr[1])));
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_10::Run_FirstPart()
{
    int x = 1;
    int currentCycle = 0;
    int nextInterestingCycle = 20;
    int stepBetweenInterestingCycles = 40;

    int interestingSignalStrenghtSum = 0;

    for (const Operation& operation : m_Operations)
    {
        if (nextInterestingCycle <= currentCycle + m_OperationDuration[operation.m_OperationType])
        {
            interestingSignalStrenghtSum += nextInterestingCycle * x;
            nextInterestingCycle += stepBetweenInterestingCycles;
        }

        ProcessOperation_1(operation, currentCycle, x);
    }

    cout << "The sum of the 6 signals' strength is " << interestingSignalStrenghtSum << "." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_10::CleanUp_FirstPart()
{
    m_Operations.clear();
    m_OperationDuration.clear();
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_10::SetUp_SecondPart()
{
	vector<string> lines;
	EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
	if (readErrorCode != EErrorCode::Success)
	{
		return readErrorCode;
	}

	m_OperationDuration[OperationType::noop] = 1;
	m_OperationDuration[OperationType::addx] = 2;

	m_CyclePerLine = 40;

	for (const string& line : lines)
	{
		vector<string> operationStr;
		FileHelper::SplitLine(line, " ", operationStr);

		if (operationStr.size() == 1)
		{
			m_Operations.push_back(Operation(OperationType::noop));
		}
		else
		{
			m_Operations.push_back(Operation(OperationType::addx, stoi(operationStr[1])));
		}
	}

	return EErrorCode::Success;
}

EErrorCode CChallenge_10::Run_SecondPart()
{
	int x = 1;
	int currentCycle = 0;

	for (const Operation& operation : m_Operations)
	{
		ProcessOperation_2(operation, currentCycle, x);
	}

    return EErrorCode::Success;
}

EErrorCode CChallenge_10::CleanUp_SecondPart()
{
	m_Operations.clear();
	m_OperationDuration.clear();
	return EErrorCode::Success;
}

void CChallenge_10::ProcessOperation_1(const Operation& _operation, int& _currentCycle, int& _x)
{
	switch (_operation.m_OperationType)
	{
	case OperationType::addx:
		_x += _operation.m_Operand;
		break;
	default:
		break;
	}

	_currentCycle += m_OperationDuration[_operation.m_OperationType];
}

void CChallenge_10::ProcessOperation_2(const Operation& _operation, int& _currentCycle, int& _x)
{
	int cyclesBeforeOperationCompletion = m_OperationDuration[_operation.m_OperationType];
	int posInRow = _currentCycle % m_CyclePerLine;

	for (int i = 0; i < cyclesBeforeOperationCompletion; ++i)
	{
		if (abs(posInRow - _x) < 2)
			cout << '#';
		else
			cout << '.';

		++_currentCycle;

		++posInRow;
		if (posInRow == m_CyclePerLine)
			cout << endl;
	}

	switch (_operation.m_OperationType)
	{
	case OperationType::addx:
		_x += _operation.m_Operand;
		break;
	default:
		break;
	}
}
