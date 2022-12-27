#pragma once

#include "Common/Challenge.h"

#include <map>
#include <set>


using namespace std;

class CChallenge_14 : public CChallenge
{
private:
    struct Position
    {
        Position() {}
        Position(const string& _str);
        Position(int _x, int _y) : x(_x), y(_y) {}

        bool operator==(const Position& _other) const { return x == _other.x && y == _other.y; }

        int x = 0;
        int y = 0;
    };

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    void AddRockSegment(const Position& _firstPos, const Position& _secondPos);
    void AddSandUnit(const Position& _sandPos);

    bool MoveSandUnit(Position& _sandPos);

#ifdef _DEBUG
    void PrintRockMap() const;
#endif // _DEBUG

    static string const sm_inputFilePath;

    map<int, set<int>> m_RockMap;
    int m_DeepestRow = 0;
    bool m_HasFloor = false;

#ifdef _DEBUG
    int m_TopBound = 0;
    int m_LeftBound = 500;
    int m_RightBound = 500;
#endif // _DEBUG
};
