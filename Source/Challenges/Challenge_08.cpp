#include "Challenge_08.h"
#include "../Helpers/FileHelpers.h"

#include <map>

////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_08::sm_inputFilePath = "Inputs/Input_Challenge_08.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_08::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_TreeMap.resize(lines.size());
    for (int i = 0; i < lines.size(); ++i)
    {
        const string& line = lines[i];

        vector<int>& treeRow = m_TreeMap[i];
        treeRow.reserve(line.size());

        for (const char& treeHeight : line)
        {
            treeRow.push_back(treeHeight - '0');
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_08::Run_FirstPart()
{
    int visibleTreesCount = 0;

    // Trees on the border
    visibleTreesCount += 4 * (int)m_TreeMap.size() - 4;

    // Inner square
    for (int rowIdx = 1; rowIdx < m_TreeMap.size() - 1; ++rowIdx)
    {
        const vector<int>& row = m_TreeMap[rowIdx];
        for (int colIdx = 1; colIdx < row.size() - 1; ++colIdx)
        {
            if (m_TreeMap[rowIdx][colIdx] == 0)
                continue;
                        
            bool isTreeVisible = false;
            for (int i = 0; i < (int)EDirection::Count; ++i)
            {
                if (IsVisibleFrom((EDirection)i, rowIdx, colIdx))
                {
                    isTreeVisible = true;
                    ++visibleTreesCount;
                    break;
                }
            }
        }
    }

#ifndef BENCHMARK
    cout << "There are " << visibleTreesCount << " trees visible from outside the grid." << endl;
#endif // !BENCHMARK

    return EErrorCode::Success;
}

EErrorCode CChallenge_08::CleanUp_FirstPart()
{
    m_TreeMap.clear();
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_08::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_TreeMap.resize(lines.size());
    for (int i = 0; i < lines.size(); ++i)
    {
        const string& line = lines[i];

        vector<int>& treeRow = m_TreeMap[i];
        treeRow.reserve(line.size());

        for (const char& treeHeight : line)
        {
            treeRow.push_back(treeHeight - '0');
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_08::Run_SecondPart()
{
    int highestScenicScore = 0;

    for (int rowIdx = 1; rowIdx < m_TreeMap.size() - 1; ++rowIdx)
    {
        const vector<int>& row = m_TreeMap[rowIdx];
        for (int colIdx = 1; colIdx < row.size() - 1; ++colIdx)
        {
            int scenicScore = ComputeScenicScore(rowIdx, colIdx);
            if (scenicScore > highestScenicScore)
                highestScenicScore = scenicScore;
        }
    }

#ifndef BENCHMARK
    cout << "The highest scenic score is " << highestScenicScore << "." << endl;
#endif // !BENCHMARK

    return EErrorCode::Success;
}

EErrorCode CChallenge_08::CleanUp_SecondPart()
{
    m_TreeMap.clear();
    return EErrorCode::Success;
}

bool CChallenge_08::IsVisibleFrom(EDirection _direction, int _rowIdx, int _colIdx)
{
    int currentTreeHeight = m_TreeMap[_rowIdx][_colIdx];
    switch (_direction)
    {
    case CChallenge_08::EDirection::Left:
    {
        const vector<int>& row = m_TreeMap[_rowIdx];
        for (size_t i = 0; i < _colIdx; ++i)
        {
            if (row[i] >= currentTreeHeight)
                return false;
        }
        break;
    }

    case CChallenge_08::EDirection::Right:
    {
        const vector<int>& row = m_TreeMap[_rowIdx];
        for (size_t i = row.size() - 1; i > _colIdx; --i)
        {
            if (row[i] >= currentTreeHeight)
                return false;
        }
        break;
    }

    case CChallenge_08::EDirection::Up:
    {
        for (size_t i = 0; i < _rowIdx; ++i)
        {
            if (m_TreeMap[i][_colIdx] >= currentTreeHeight)
                return false;
        }
        break;
    }

    case CChallenge_08::EDirection::Down:
    {
        for (size_t i = m_TreeMap.size() - 1; i > _rowIdx; --i)
        {
            if (m_TreeMap[i][_colIdx] >= currentTreeHeight)
                return false;
        }
        break;
    }

    default:
        break;
    }

    return true;
}

void CChallenge_08::ComputeDirectionCheckOrder(int _row, int _col, vector<EDirection>& _outDirectionOrder)
{
    map<EDirection, int> distanceToCheckPerDirection;
    distanceToCheckPerDirection[EDirection::Left] = _col;
    distanceToCheckPerDirection[EDirection::Right] = (int)m_TreeMap.size() - _col - 1;
    distanceToCheckPerDirection[EDirection::Up] = _row;
    distanceToCheckPerDirection[EDirection::Down] = (int)m_TreeMap.size() - _row - 1;

    for (map<EDirection, int>::const_iterator distanceIt = distanceToCheckPerDirection.begin(); distanceIt != distanceToCheckPerDirection.end(); ++distanceIt)
    {
        if (_outDirectionOrder.empty())
        {
            _outDirectionOrder.push_back(distanceIt->first);
        }
        else
        {
            bool wasInserted = false;
            for (vector<EDirection>::const_iterator directionIt = _outDirectionOrder.begin(); directionIt != _outDirectionOrder.end(); ++directionIt)
            {
                if (distanceToCheckPerDirection[*directionIt] > distanceIt->second)
                {
                    _outDirectionOrder.insert(directionIt, distanceIt->first);
                    wasInserted = true;
                    break;
                }
            }
            if (!wasInserted)
            {
                _outDirectionOrder.push_back(distanceIt->first);
            }
        }
    }
}

int CChallenge_08::ComputeScenicScore(int _rowIdx, int _colIdx)
{
    int currentTreeHeight = m_TreeMap[_rowIdx][_colIdx];
    int scenicScore = 1;

    for (int dirIdx = 0; dirIdx < (int)EDirection::Count && scenicScore > 0; ++dirIdx)
    {
        switch ((EDirection)dirIdx)
        {
        case CChallenge_08::EDirection::Left:
        {
            const vector<int>& row = m_TreeMap[_rowIdx];
            int directionScore = 0;
            for (int i = _colIdx - 1; i >= 0; --i)
            {
                ++directionScore;

                if (row[i] >= currentTreeHeight)
                    break;
            }

            scenicScore *= directionScore;
            break;
        }

        case CChallenge_08::EDirection::Right:
        {
            const vector<int>& row = m_TreeMap[_rowIdx];
            int directionScore = 0;
            for (int i = _colIdx + 1; i < row.size(); ++i)
            {
                ++directionScore;

                if (row[i] >= currentTreeHeight)
                    break;
            }

            scenicScore *= directionScore;
            break;
        }

        case CChallenge_08::EDirection::Up:
        {
            int directionScore = 0;
            for (int i = _rowIdx - 1; i >= 0; --i)
            {
                ++directionScore;

                if (m_TreeMap[i][_colIdx] >= currentTreeHeight)
                    break;
            }

            scenicScore *= directionScore;
            break;
        }

        case CChallenge_08::EDirection::Down:
        {
            int directionScore = 0;
            for (int i = _rowIdx + 1; i < m_TreeMap.size(); ++i)
            {
                ++directionScore;

                if (m_TreeMap[i][_colIdx] >= currentTreeHeight)
                    break;
            }

            scenicScore *= directionScore;
            break;
        }

        default:
            break;
        }
    }

    return scenicScore;
}
