#include "Challenge_05.h"
#include "../Helpers/FileHelpers.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_05::sm_inputFilePath = "Inputs/Input_Challenge_05.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_05::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    // Find the empty line that separates the crate states and the moving instructions
    size_t emptyLineIndex = 0;
    for (size_t i = 0; i < lines.size(); ++i)
    {
        if (lines[i].empty())
        {
            emptyLineIndex = i;
            break;
        }
    }

    // Count amount of stacks
    const size_t stackNumbersLineSize = lines[emptyLineIndex - 1].size();
    const size_t stackCount = (stackNumbersLineSize + 1) / 4;

    m_CrateStacks.resize(stackCount);

    // Fill crate stacks data
    for (size_t i = emptyLineIndex - 2; ; --i)
    {
        const string currentLine = lines[i];

        const size_t firstStackLetterIndex = 1;
        for (size_t j = 0; j < stackCount; ++j)
        {
            const size_t currentLetterIndex = firstStackLetterIndex + j * 4;
            const char crateLetter = currentLine[currentLetterIndex];

            if (!isalpha(crateLetter))
                continue;

            m_CrateStacks[j].push(crateLetter);
        }

        if (i == 0)
            break;
    }

    // Store moving instructions
    m_MovingSteps.reserve(lines.size() - emptyLineIndex - 1);
    for (size_t i = emptyLineIndex + 1; i < lines.size(); ++i)
    {
        string stepLine = lines[i];

        stepLine.erase(0, 5);
        stepLine.erase(stepLine.find("from "), 5);
        stepLine.erase(stepLine.find("to "), 3);

        vector<string> stepNumbers;
        FileHelper::SplitLine(stepLine, " ", stepNumbers);
        m_MovingSteps.push_back(MovingStep(stoi(stepNumbers[0]), stoi(stepNumbers[1]) - 1, stoi(stepNumbers[2]) - 1));
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_05::Run_FirstPart()
{
    for (const MovingStep& step : m_MovingSteps)
    {
        for (size_t i = 0; i < step.m_Move; ++i)
        {
            m_CrateStacks[step.m_To].push(m_CrateStacks[step.m_From].top());
            m_CrateStacks[step.m_From].pop();
        }
    }

    cout << "The following crates end up on top of the stacks : ";
    for (const stack<char>& crateStack : m_CrateStacks)
    {
        cout << crateStack.top();
    }

    cout << "." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_05::CleanUp_FirstPart()
{
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_05::SetUp_SecondPart()
{
	vector<string> lines;
	EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
	if (readErrorCode != EErrorCode::Success)
	{
		return readErrorCode;
	}

	// Find the empty line that separates the crate states and the moving instructions
	size_t emptyLineIndex = 0;
	for (size_t i = 0; i < lines.size(); ++i)
	{
		if (lines[i].empty())
		{
			emptyLineIndex = i;
			break;
		}
	}

	// Count amount of stacks
	const size_t stackNumbersLineSize = lines[emptyLineIndex - 1].size();
	const size_t stackCount = (stackNumbersLineSize + 1) / 4;

	m_CrateStacks.resize(stackCount);

	// Fill crate stacks data
	for (size_t i = emptyLineIndex - 2; ; --i)
	{
		const string currentLine = lines[i];

		const size_t firstStackLetterIndex = 1;
		for (size_t j = 0; j < stackCount; ++j)
		{
			const size_t currentLetterIndex = firstStackLetterIndex + j * 4;
			const char crateLetter = currentLine[currentLetterIndex];

			if (!isalpha(crateLetter))
				continue;

			m_CrateStacks[j].push(crateLetter);
		}

		if (i == 0)
			break;
	}

	// Store moving instructions
	m_MovingSteps.reserve(lines.size() - emptyLineIndex - 1);
	for (size_t i = emptyLineIndex + 1; i < lines.size(); ++i)
	{
		string stepLine = lines[i];

		stepLine.erase(0, 5);
		stepLine.erase(stepLine.find("from "), 5);
		stepLine.erase(stepLine.find("to "), 3);

		vector<string> stepNumbers;
		FileHelper::SplitLine(stepLine, " ", stepNumbers);
		m_MovingSteps.push_back(MovingStep(stoi(stepNumbers[0]), stoi(stepNumbers[1]) - 1, stoi(stepNumbers[2]) - 1));
	}

	return EErrorCode::Success;
}

EErrorCode CChallenge_05::Run_SecondPart()
{
	for (const MovingStep& step : m_MovingSteps)
	{
        stack<char> cratesToMove;
		for (size_t i = 0; i < step.m_Move; ++i)
		{
            cratesToMove.push(m_CrateStacks[step.m_From].top());
			m_CrateStacks[step.m_From].pop();
		}

        for (size_t i = 0; i < step.m_Move; ++i)
        {
			m_CrateStacks[step.m_To].push(cratesToMove.top());
            cratesToMove.pop();
        }
	}

	cout << "The following crates end up on top of the stacks : ";
	for (const stack<char>& crateStack : m_CrateStacks)
	{
		cout << crateStack.top();
	}

	cout << "." << endl;

	return EErrorCode::Success;
}

EErrorCode CChallenge_05::CleanUp_SecondPart()
{
    return EErrorCode::Success;
}
