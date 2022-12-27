#include "Challenge_14.h"
#include "../Helpers/FileHelpers.h"

#include <iomanip>

////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_14::sm_inputFilePath = "Inputs/Input_Challenge_14.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_14::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    for (const string& line : lines)
    {
        vector<string> cornerStr;
        FileHelper::SplitLine(line, " -> ", cornerStr);

        Position firstPos(cornerStr[0]);
        for (int i = 1; i < cornerStr.size(); ++i)
        {
            Position secondPos(cornerStr[i]);

            AddRockSegment(firstPos, secondPos);

            firstPos = secondPos;
        }
    }

#ifdef _DEBUG
    cout << "Initial map:" << endl;
    PrintRockMap();
    cout << endl;
#endif // _DEBUG

    return EErrorCode::Success;
}

EErrorCode CChallenge_14::Run_FirstPart()
{
    int sandUnits = 0;
    bool done = false;
    while (!done)
    {
        Position sandPos(500, 0);

        // Move the sand unit as long as it's not resting
        while (MoveSandUnit(sandPos))
        {
            // If the unit reaches the deepest rock, we're done
            if (sandPos.y >= m_DeepestRow)
            {
                done = true;
                break;
            }
        }

        if (!done)
        {
            ++sandUnits;
        }
    }

#ifndef BENCHMARK
    cout << sandUnits << " sand units came to rest before the sand started flowing into the abyss below." << endl;
#endif // !BENCHMARK

#ifdef _DEBUG
    cout << "Final map:" << endl;
    PrintRockMap();
    cout << endl;
#endif // _DEBUG

    return EErrorCode::Success;
}

EErrorCode CChallenge_14::CleanUp_FirstPart()
{
    m_RockMap.clear();
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_14::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    for (const string& line : lines)
    {
        vector<string> cornerStr;
        FileHelper::SplitLine(line, " -> ", cornerStr);

        Position firstPos(cornerStr[0]);
        for (int i = 1; i < cornerStr.size(); ++i)
        {
            Position secondPos(cornerStr[i]);

            AddRockSegment(firstPos, secondPos);

            firstPos = secondPos;
        }
    }

    m_HasFloor = true;

#ifdef _DEBUG
    cout << "Initial map:" << endl;
    PrintRockMap();
    cout << endl;
#endif // _DEBUG

    return EErrorCode::Success;
}

EErrorCode CChallenge_14::Run_SecondPart()
{
    int sandUnits = 0;
    bool done = false;
    const Position initialSandPos(500, 0);
    while (!done)
    {
        Position sandPos(initialSandPos);

        // Move the sand unit as long as it's not resting
        while (MoveSandUnit(sandPos));

        ++sandUnits;

        // If the unit cannot move from its starting point, we're done
        if (sandPos == initialSandPos)
        {
            done = true;
            break;
        }
    }

#ifndef BENCHMARK
    cout << sandUnits << " sand units came to rest before the a sand unit was unable to move from the starting point." << endl;
#endif // !BENCHMARK

#ifdef _DEBUG
    cout << "Final map:" << endl;
    PrintRockMap();
    cout << endl;
#endif // _DEBUG

    return EErrorCode::Success;
}

EErrorCode CChallenge_14::CleanUp_SecondPart()
{
    m_RockMap.clear();
    return EErrorCode::Success;
}


void CChallenge_14::AddRockSegment(const Position& _firstPos, const Position& _secondPos)
{
    if (_firstPos.x == _secondPos.x)
    {
        int startY = min(_firstPos.y, _secondPos.y);
        int endY = max(_firstPos.y, _secondPos.y);
        for (int i = startY; i <= endY; ++i)
        {
            m_RockMap[i].insert(_firstPos.x);
        }

        if (m_DeepestRow < endY)
            m_DeepestRow = endY;

#ifdef _DEBUG
        if (startY < m_TopBound)
            m_TopBound = startY;

        if (_firstPos.x < m_LeftBound)
            m_LeftBound = _firstPos.x;

        if (_firstPos.x > m_RightBound)
            m_RightBound = _firstPos.x;
#endif // _DEBUG
    }
    else
    {
        int startX = min(_firstPos.x, _secondPos.x);
        int endX = max(_firstPos.x, _secondPos.x);

        set<int>& row = m_RockMap[_firstPos.y];
        for (int i = startX; i <= endX; ++i)
        {
            row.insert(i);
        }

        if (m_DeepestRow < _firstPos.y)
            m_DeepestRow = _firstPos.y;

#ifdef _DEBUG
        if (_firstPos.y < m_TopBound)
            m_TopBound = _firstPos.y;

        if (startX < m_LeftBound)
            m_LeftBound = startX;

        if (endX > m_RightBound)
            m_RightBound = endX;
#endif // _DEBUG
    }
}


void CChallenge_14::AddSandUnit(const Position& _sandPos)
{
    m_RockMap[_sandPos.y].insert(_sandPos.x);

#ifdef _DEBUG
    m_LeftBound = min(m_LeftBound, _sandPos.x);
    m_RightBound = max(m_RightBound, _sandPos.x);

    static bool printMap = false;
    if (printMap)
    {
        PrintRockMap();
        cout << endl;
    }
#endif
}

bool CChallenge_14::MoveSandUnit(Position& _sandPos)
{
    const int rowIdx = _sandPos.y + 1;

    if (m_HasFloor && rowIdx == m_DeepestRow + 2)
    {
        // Floor underneath. Sand unit is resting.
        AddSandUnit(_sandPos);
        return false;
    }

    map<int, set<int>>::const_iterator rowIt = m_RockMap.find(rowIdx);

    if (rowIt == m_RockMap.end())
    {
        // Empty row underneath. Move sand unit.
        _sandPos.y = rowIdx;
        return true;
    }

    const set<int>& row = rowIt->second;

    vector<int> xOffsets{ 0, -1, 1 };

    int colIdx = 0;
    for (const int& offset : xOffsets)
    {
        colIdx = _sandPos.x + offset;

        if (row.find(colIdx) == row.end())
        {
            // Empty space found. Move sand unit.
            _sandPos = Position(colIdx, rowIdx);
            return true;
        }
    }

    // No space found underneath. Sand unit is resting.
    AddSandUnit(_sandPos);
    return false;
}

#ifdef _DEBUG
void CChallenge_14::PrintRockMap() const
{
    int lineWidth = m_RightBound - m_LeftBound + 1;

    int sandPos = 500 - m_LeftBound;
    cout << "000 ";
    cout << string(sandPos, '.') << "+" << string(lineWidth - sandPos - 1, '.') << endl;

    int previousY = m_TopBound;
    for (map<int, set<int>>::const_iterator rowIt = m_RockMap.begin(); rowIt != m_RockMap.end(); ++rowIt)
    {
        int currentY = rowIt->first;


        for (int i = previousY + 1; i < currentY; ++i)
        {
            cout << setfill('0') << setw(3) << i << " ";
            cout << string(lineWidth, '.') << endl;
        }

        cout << setfill('0') << setw(3) << currentY << " ";

        const set<int>& row = rowIt->second;
        int previousX = m_LeftBound - 1;
        for (set<int>::const_iterator colIt = row.begin(); colIt != row.end(); ++colIt)
        {
            int currentX = *colIt;
            cout << string(currentX - previousX - 1, '.') << "#";
            previousX = currentX;
        }

        cout << string(m_RightBound - previousX, '.');

        cout << endl;

        previousY = currentY;
    }

    if (m_HasFloor)
    {
        for (int i = previousY + 1; i < m_DeepestRow + 2; ++i)
        {
            cout << setfill('0') << setw(3) << previousY + 1 << " ";
            cout << string(lineWidth, '.') << endl;
        }

        cout << setfill('0') << setw(3) << previousY + 2 << " ";
        cout << string(lineWidth, '#') << endl;
    }
}
#endif // _DEBUG

CChallenge_14::Position::Position(const string& _str)
{
    vector<string> positionStr;
    FileHelper::SplitLine(_str, ",", positionStr);

    x = stoi(positionStr[0]);
    y = stoi(positionStr[1]);
}
