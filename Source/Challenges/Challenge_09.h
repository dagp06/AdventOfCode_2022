#pragma once

#include "Common/Challenge.h"

using namespace std;


class CChallenge_09 : public CChallenge
{
public:
    struct Position
    {
        int x = 0;
        int y = 0;

        bool operator<(const Position& _other) const { return x < _other.x || (x == _other.x && y < _other.y); }
        bool operator==(const Position& _other) const { return x == _other.x && y == _other.y; }

        bool IsTouching(const Position& _pos) { return abs(x - _pos.x) < 2 && abs(y - _pos.y) < 2; }
    };

private:

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    void MoveHead(Position& _headPos, const char& _direction);
    void MoveTail(Position& _tailPos, const Position& _headPos);

    static string const sm_inputFilePath;

    typedef pair<char, int> Motion;
    vector<Motion> m_Motions;
};
