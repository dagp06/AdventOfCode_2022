#include "Challenge_06.h"
#include "../Helpers/FileHelpers.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_06::sm_inputFilePath = "Inputs/Input_Challenge_06.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_06::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_DataStreamBuffer = lines[0];

    return EErrorCode::Success;
}

EErrorCode CChallenge_06::Run_FirstPart()
{
    cout << "The first start-of-packer marker is detected after processing " << ComputeCharsToProcessToFindMarker(4) << " characters" << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_06::CleanUp_FirstPart()
{
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_06::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_DataStreamBuffer = lines[0];

    return EErrorCode::Success;
}

EErrorCode CChallenge_06::Run_SecondPart()
{
    return EErrorCode::Success;
}

EErrorCode CChallenge_06::CleanUp_SecondPart()
{
    cout << "The first start-of-packer marker is detected after processing " << ComputeCharsToProcessToFindMarker(14) << " characters" << endl;

    return EErrorCode::Success;
}

int CChallenge_06::ComputeCharsToProcessToFindMarker(int _markerLength)
{
    int endOfMarkerIdx = 0;

    for (int i = 0; i < m_DataStreamBuffer.size();)
    {
        endOfMarkerIdx = i + _markerLength - 1;
        bool sameCharactersFound = false;

        for (int j = i; j < endOfMarkerIdx; ++j)
        {
            for (int k = j + 1; k <= endOfMarkerIdx; ++k)
            {
                if (m_DataStreamBuffer[j] == m_DataStreamBuffer[k])
                {
                    sameCharactersFound = true;
                    i = j + 1; // The char at pos j is also present within the next [_markerLength] chars. The next char to test (i) should be the one after the one at pos j.
                    break;
                }
            }

            if (sameCharactersFound)
                break;
        }

        if (!sameCharactersFound)
            break; // We found the start-of-packet marker !
    }

    return endOfMarkerIdx + 1;
}
