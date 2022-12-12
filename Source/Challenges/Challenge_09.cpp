#include "Challenge_09.h"
#include "../Helpers/FileHelpers.h"

#include <set>

////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_09::sm_inputFilePath = "Inputs/Input_Challenge_09.txt";


////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_09::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    for (const string& line : lines)
    {
        vector<string> motionStrings;
        FileHelper::SplitLine(line, " ", motionStrings);

        m_Motions.push_back(Motion(motionStrings[0][0], stoi(motionStrings[1])));
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_09::Run_FirstPart()
{
    Position headPos;
    Position tailPos;

    set<Position> positionsVisitedByTail;
    positionsVisitedByTail.insert(tailPos);

    for (const Motion& motion : m_Motions)
    {
        for (int i = 0; i < motion.second; ++i)
        {
            MoveHead(headPos, motion.first);

            if (!headPos.IsTouching(tailPos))
            {
                MoveTail(tailPos, headPos);
                positionsVisitedByTail.insert(tailPos);
            }
        }
    }

#ifndef BENCHMARK
    cout << "The tail of the rope visited " << positionsVisitedByTail.size() << " positions." << endl;
#endif // !BENCHMARK

    return EErrorCode::Success;
}

EErrorCode CChallenge_09::CleanUp_FirstPart()
{
    m_Motions.clear();
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_09::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    for (const string& line : lines)
    {
        vector<string> motionStrings;
        FileHelper::SplitLine(line, " ", motionStrings);

        m_Motions.push_back(Motion(motionStrings[0][0], stoi(motionStrings[1])));
    }
    
    return EErrorCode::Success;
}

EErrorCode CChallenge_09::Run_SecondPart()
{
    const int ropeNodeCount = 10;
    vector<Position> ropeNodes;
    ropeNodes.resize(ropeNodeCount);
    Position& headPos = ropeNodes[0];

    set<Position> positionsVisitedByTail;
    positionsVisitedByTail.insert(headPos);

    for (const Motion& motion : m_Motions)
    {
        for (int i = 0; i < motion.second; ++i)
        {
            MoveHead(headPos, motion.first);

            for (int i = 1; i < ropeNodeCount; ++i)
            {
                Position& currentHeadPos = ropeNodes[i - 1];
                Position& currentTailPos = ropeNodes[i];
                if (!currentHeadPos.IsTouching(currentTailPos))
                {
                    MoveTail(currentTailPos, currentHeadPos);

                    // Rope's tail moved. Store potentially new position.
                    if (i == ropeNodeCount - 1)
                        positionsVisitedByTail.insert(currentTailPos);
                }
            }
        }
    }

#ifndef BENCHMARK
    cout << "The tail of the rope visited " << positionsVisitedByTail.size() << " positions." << endl;
#endif // !BENCHMARK

    return EErrorCode::Success;
}

EErrorCode CChallenge_09::CleanUp_SecondPart()
{
    m_Motions.clear();
    return EErrorCode::Success;
}


void CChallenge_09::MoveHead(Position& _headPos, const char& _direction)
{
    switch (_direction)
    {
    case 'U':
        _headPos.y += 1;
        break;
    case 'D':
        _headPos.y -= 1;
        break;
    case 'L':
        _headPos.x -= 1;
        break;
    case 'R':
        _headPos.x += 1;
        break;
    default:
        break;
    }
}

void CChallenge_09::MoveTail(Position& _tailPos, const Position& _headPos)
{
    int xDiff = _headPos.x - _tailPos.x;
    if (xDiff > 0)
    {
        ++_tailPos.x;
    }
    else if (xDiff < 0)
    {
        --_tailPos.x;
    }

    int yDiff = _headPos.y - _tailPos.y;
    if (yDiff > 0)
    {
        ++_tailPos.y;
    }
    else if (yDiff < 0)
    {
        --_tailPos.y;
    }
}
