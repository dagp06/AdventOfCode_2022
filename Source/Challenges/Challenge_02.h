#pragma once

#include "Common/Challenge.h"
#include <map>

using namespace std;

class CChallenge_02 : public CChallenge
{
private:
    enum class ERoundOutcome
    {
        Undefined,
        Win,
        Draw,
        Lose
    };

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    int ComputeRoundScore_Part1(const char& _opponentMove, const char& _myMove);
    int ComputeRoundScore_Part2(const char& _opponentMove, const ERoundOutcome& _roundOutcome);
    ERoundOutcome ComputeRoundOutcome(const char& _opponentMove, const char& _myMove);
    char ComputeMyMove(const char& _opponentMove, const ERoundOutcome& _roundOutcome);

    static string const sm_inputFilePath;

    vector<char> m_OpponentMoves;
    vector<char> m_MyMoves;
    vector<ERoundOutcome> m_RequiredOutcome;

    map<char, int> m_MyMoveScoreMap;
    map<ERoundOutcome, int> m_RoundOutcomeScoreMap;
    map<char, ERoundOutcome> m_CharToRoundOutcomeMap;
};
