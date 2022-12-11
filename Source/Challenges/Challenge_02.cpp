#include "Challenge_02.h"
#include "../Helpers/FileHelpers.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_02::sm_inputFilePath = "Inputs/Input_Challenge_02.txt";

////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_02::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    for (const string& line : lines)
    {
        m_OpponentMoves.push_back(line[0]);
        m_MyMoves.push_back(line[2]);
    }

    m_MyMoveScoreMap['X'] = 1;
    m_MyMoveScoreMap['Y'] = 2;
    m_MyMoveScoreMap['Z'] = 3;

    m_RoundOutcomeScoreMap[ERoundOutcome::Undefined] = 0;
    m_RoundOutcomeScoreMap[ERoundOutcome::Win] = 6;
    m_RoundOutcomeScoreMap[ERoundOutcome::Draw] = 3;
    m_RoundOutcomeScoreMap[ERoundOutcome::Lose] = 0;

    return EErrorCode::Success;
}

EErrorCode CChallenge_02::Run_FirstPart()
{
    int totalScore = 0;
    for (int i = 0; i < m_OpponentMoves.size(); ++i)
    {
        totalScore += ComputeRoundScore_Part1(m_OpponentMoves[i], m_MyMoves[i]);
    }

    cout << "The total score is " << totalScore << "." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_02::CleanUp_FirstPart()
{
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_02::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_CharToRoundOutcomeMap['X'] = ERoundOutcome::Lose;
    m_CharToRoundOutcomeMap['Y'] = ERoundOutcome::Draw;
    m_CharToRoundOutcomeMap['Z'] = ERoundOutcome::Win;

    for (const string& line : lines)
    {
        m_OpponentMoves.push_back(line[0]);
        m_RequiredOutcome.push_back(m_CharToRoundOutcomeMap[line[2]]);
    }

    m_MyMoveScoreMap['X'] = 1;
    m_MyMoveScoreMap['Y'] = 2;
    m_MyMoveScoreMap['Z'] = 3;

    m_RoundOutcomeScoreMap[ERoundOutcome::Undefined] = 0;
    m_RoundOutcomeScoreMap[ERoundOutcome::Win] = 6;
    m_RoundOutcomeScoreMap[ERoundOutcome::Draw] = 3;
    m_RoundOutcomeScoreMap[ERoundOutcome::Lose] = 0;

    return EErrorCode::Success;
}

EErrorCode CChallenge_02::Run_SecondPart()
{
    int totalScore = 0;
    for (int i = 0; i < m_OpponentMoves.size(); ++i)
    {
        totalScore += ComputeRoundScore_Part2(m_OpponentMoves[i], m_RequiredOutcome[i]);
    }

    cout << "The total score is " << totalScore << "." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_02::CleanUp_SecondPart()
{
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////

int CChallenge_02::ComputeRoundScore_Part1(const char& _opponentMove, const char& _myMove)
{
    int score = 0;

    // My shape's score
    score += m_MyMoveScoreMap[_myMove];

    // Round outcome score
    score += m_RoundOutcomeScoreMap[ComputeRoundOutcome(_opponentMove, _myMove)];

    return score;
}


int CChallenge_02::ComputeRoundScore_Part2(const char& _opponentMove, const ERoundOutcome& _roundOutcome)
{
    int score = 0;

    // My shape's score
    score += m_MyMoveScoreMap[ComputeMyMove(_opponentMove, _roundOutcome)];

    // Round outcome score
    score += m_RoundOutcomeScoreMap[_roundOutcome];

    return score;
}

CChallenge_02::ERoundOutcome CChallenge_02::ComputeRoundOutcome(const char& _opponentMove, const char& _myMove)
{
    ERoundOutcome roundOutcome = ERoundOutcome::Undefined;
    switch (_opponentMove)
    {
    case 'A':
    {
        switch (_myMove)
        {
        case 'X':
            roundOutcome = ERoundOutcome::Draw;
            break;
        case 'Y':
            roundOutcome = ERoundOutcome::Win;
            break;
        case 'Z':
            roundOutcome = ERoundOutcome::Lose;
        default:
            break;
        }
        break;
    }

    case 'B':
    {
        switch (_myMove)
        {
        case 'X':
            roundOutcome = ERoundOutcome::Lose;
            break;
        case 'Y':
            roundOutcome = ERoundOutcome::Draw;
            break;
        case 'Z':
            roundOutcome = ERoundOutcome::Win;
        default:
            break;
        }
        break;
    }

    case 'C':
    {
        switch (_myMove)
        {
        case 'X':
            roundOutcome = ERoundOutcome::Win;
            break;
        case 'Y':
            roundOutcome = ERoundOutcome::Lose;
            break;
        case 'Z':
            roundOutcome = ERoundOutcome::Draw;
        default:
            break;
        }
        break;
    }
    default:
        break;
    }

    return roundOutcome;
}

char CChallenge_02::ComputeMyMove(const char& _opponentMove, const ERoundOutcome& _roundOutcome)
{
    char myMove;
    switch (_opponentMove)
    {
    case 'A':
    {
        switch (_roundOutcome)
        {
        case CChallenge_02::ERoundOutcome::Win:
            myMove = 'Y';
            break;
        case CChallenge_02::ERoundOutcome::Draw:
            myMove = 'X';
            break;
        case CChallenge_02::ERoundOutcome::Lose:
            myMove = 'Z';
            break;
        default:
            break;
        }
        break;
    }

    case 'B':
    {
        switch (_roundOutcome)
        {
        case CChallenge_02::ERoundOutcome::Win:
            myMove = 'Z';
            break;
        case CChallenge_02::ERoundOutcome::Draw:
            myMove = 'Y';
            break;
        case CChallenge_02::ERoundOutcome::Lose:
            myMove = 'X';
            break;
        default:
            break;
        }
        break;
    }

    case 'C':
    {
        switch (_roundOutcome)
        {
        case CChallenge_02::ERoundOutcome::Win:
            myMove = 'X';
            break;
        case CChallenge_02::ERoundOutcome::Draw:
            myMove = 'Z';
            break;
        case CChallenge_02::ERoundOutcome::Lose:
            myMove = 'Y';
            break;
        default:
            break;
        }
        break;
    }
    }

    return myMove;
}
