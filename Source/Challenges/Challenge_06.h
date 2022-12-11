#pragma once

#include "Common/Challenge.h"

using namespace std;

class CChallenge_06 : public CChallenge
{
private:
    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    int ComputeCharsToProcessToFindMarker(int _markerLength);

    static string const sm_inputFilePath;

    string m_DataStreamBuffer;
};
