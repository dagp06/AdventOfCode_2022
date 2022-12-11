#pragma once

#include "Common/Challenge.h"

using namespace std;

class CChallenge_08 : public CChallenge
{
private:
    enum class EDirection
    {
        Left,
        Right,
        Up,
        Down,
        Count
    };

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    bool IsVisibleFrom(EDirection _direction, int _rowIdx, int _colIdx);
    void ComputeDirectionCheckOrder(int _rowIdx, int _colIdx, vector<EDirection>& _outDirectionOrder);
    int ComputeScenicScore(int _rowIdx, int _colIdx);

    static string const sm_inputFilePath;

    vector<vector<int>> m_TreeMap;
};
