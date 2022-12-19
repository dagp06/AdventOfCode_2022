#include "Challenge_12.h"
#include "../Helpers/FileHelpers.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_12::sm_inputFilePath = "Inputs/Input_Challenge_12.txt";




////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_12::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_HeightMap.resize(lines.size());

#ifdef _DEBUG
    m_DebugVisitedMap.resize(lines.size());
#endif

    for (int i = 0; i < lines.size(); ++i)
    {
        const string& line = lines[i];

#ifdef _DEBUG
        m_DebugVisitedMap[i].resize(line.size());
#endif

        vector<char>& row = m_HeightMap[i];
        row.resize(line.size());
        for (int j = 0; j < line.size(); ++j)
        {
            const char& elevation = line[j];
            if (elevation == 'S')
            {
                m_StartPos = Position(j, i);
                row[j] = 'a';
            }
            else if (elevation == 'E')
            {
                m_EndPos = Position(j, i);
                row[j] = 'z';
            }
            else
            {
                row[j] = elevation;
            }

#ifdef _DEBUG
            m_DebugVisitedMap[i][j] = false;
#endif
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_12::Run_FirstPart()
{
    int stepsCount = Solve_1();

#ifndef BENCHMARK
    cout << "It takes " << stepsCount << " steps to reach the location with the best signal." << endl;
#endif

#ifdef _DEBUG
    PrintVisitedMap();
#endif

    return EErrorCode::Success;
}

EErrorCode CChallenge_12::CleanUp_FirstPart()
{
    m_HeightMap.clear();
    m_SquaresToVisit.clear();
    m_VisitedSquares.clear();

#ifdef _DEBUG
    m_DebugVisitedMap.clear();
#endif
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_12::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_HeightMap.resize(lines.size());
    m_DistToEndMap.resize(lines.size());

#ifdef _DEBUG
    m_DebugVisitedMap.resize(lines.size());
#endif

    for (int i = 0; i < lines.size(); ++i)
    {
        const string& line = lines[i];

#ifdef _DEBUG
        m_DebugVisitedMap[i].resize(line.size());
#endif

        m_DistToEndMap[i].resize(line.size());

        vector<char>& row = m_HeightMap[i];
        row.resize(line.size());
        for (int j = 0; j < line.size(); ++j)
        {
            const char& elevation = line[j];
            if (elevation == 'S')
            {
                m_StartPos = Position(j, i);
                row[j] = 'a';
            }
            else if (elevation == 'E')
            {
                m_EndPos = Position(j, i);
                row[j] = 'z';
            }
            else
            {
                row[j] = elevation;
            }

            m_DistToEndMap[i][j] = INT_MAX;

#ifdef _DEBUG
            m_DebugVisitedMap[i][j] = false;
#endif
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_12::Run_SecondPart()
{
    Solve_2();

#ifndef BENCHMARK
    cout << "The shortest path from a starting point to the location with the best signal is " << m_DistToEndMap[m_BestStartPoint.y][m_BestStartPoint.x] << " (" << m_BestStartPoint.x << ", " << m_BestStartPoint.y << ")." << endl;
#endif

#ifdef _DEBUG
    PrintVisitedMap();
#endif

    return EErrorCode::Success;
}

EErrorCode CChallenge_12::CleanUp_SecondPart()
{
    m_HeightMap.clear();
    m_SquaresToVisit.clear();
    m_DistToEndMap.clear();

#ifdef _DEBUG
    m_DebugVisitedMap.clear();
#endif
    return EErrorCode::Success;
}

int CChallenge_12::Solve_1()
{
    int stepsTakenCount = 0;

    m_SquaresToVisit.push_back(Square(m_StartPos, 0, ComputeHeuristic_1(m_StartPos)));
    
    while(!m_SquaresToVisit.empty())
    {
        sort(m_SquaresToVisit.begin(), m_SquaresToVisit.end());
        VisitNeighboors_1(*m_SquaresToVisit.begin(), stepsTakenCount);
    }

    return stepsTakenCount;
}

void CChallenge_12::VisitNeighboors_1(const Square& _square, int& _stepsTakenCount)
{
    Square currentSquare = _square;

    m_SquaresToVisit.erase(m_SquaresToVisit.begin());

    // Update Visited List
    {
        vector<Square>::iterator visitedIt = FindPos(m_VisitedSquares, currentSquare.m_Pos);
        if (visitedIt != m_VisitedSquares.end())
        {
            if (currentSquare.m_StepsTaken < visitedIt->m_StepsTaken)
            {
                // Already visited with a higher cost, revisit
                m_VisitedSquares.erase(visitedIt);
                m_VisitedSquares.push_back(currentSquare);
#ifdef _DEBUG
                m_DebugVisitedMap[currentSquare.m_Pos.y][currentSquare.m_Pos.x] = true;
#endif
            }
            else
            {
                // Already visited with a lower cost
                return;
            }
        }
        else
        {
            m_VisitedSquares.push_back(currentSquare);
#ifdef _DEBUG
            m_DebugVisitedMap[currentSquare.m_Pos.y][currentSquare.m_Pos.x] = true;
#endif
        }
    }

    // Exit condition
    if (currentSquare.m_Pos == m_EndPos)
    {
        _stepsTakenCount = currentSquare.m_StepsTaken;
        m_SquaresToVisit.clear();
        return;
    }

    // Visit Neighboors
    vector<Position> neighboors;
    GetVisitableNeighboors_1(currentSquare.m_Pos, neighboors);
    for (const Position& neighboorPos : neighboors)
    {
        Square neighboorSquare(neighboorPos, currentSquare.m_StepsTaken + 1, ComputeHeuristic_1(neighboorPos));

        vector<Square>::iterator visitedIt = FindPos(m_VisitedSquares, neighboorSquare.m_Pos);
        if (visitedIt != m_VisitedSquares.end() && visitedIt->m_StepsTaken <= neighboorSquare.m_StepsTaken)
        {
            // Already visited with a lower or same cost
            continue;
        }

        vector<Square>::iterator toVisitIt = FindPos(m_SquaresToVisit, neighboorSquare.m_Pos);
        if (toVisitIt != m_SquaresToVisit.end())
        {
            if (neighboorSquare.m_StepsTaken < toVisitIt->m_StepsTaken)
            {
                // Already planned to visit it with a higher cost, replace cost
                m_SquaresToVisit.erase(toVisitIt);
                m_SquaresToVisit.push_back(neighboorSquare);
            }
            else
            {
                // Already planned to visit with a lower cost
                continue;
            }
        }
        else
        {
            m_SquaresToVisit.push_back(neighboorSquare);
        }
    }
}

void CChallenge_12::GetVisitableNeighboors_1(const Position& _pos, vector<Position>& _outNeighboors)
{
    const char& currentHeight = m_HeightMap[_pos.y][_pos.x];

    for (int yOffset = -1; yOffset <= 1; yOffset += 2)
    {
        int yPos = _pos.y + yOffset;
        if (yPos < 0 || yPos > m_HeightMap.size() - 1)
            continue;

        Position pos(_pos.x, yPos);
        const char& height = m_HeightMap[yPos][_pos.x];
        if (currentHeight > height || height - currentHeight < 2)
        {
            _outNeighboors.push_back(pos);
        }
    }

    const vector<char>& row = m_HeightMap[_pos.y];
    for (int xOffset = -1; xOffset <= 1; xOffset += 2)
    {
        int xPos = _pos.x + xOffset;
        if (xPos < 0 || xPos > row.size() - 1)
            continue;

        Position pos(xPos, _pos.y);
        const char& height = row[xPos];
        if (currentHeight > height || height - currentHeight < 2)
        {
            _outNeighboors.push_back(pos);
        }
    }
}

vector<CChallenge_12::Square>::iterator CChallenge_12::FindPos(vector<Square>& _squares, const Position& _pos)
{
    for (vector<Square>::iterator it = _squares.begin(); it != _squares.end(); ++it)
    {
        if (it->m_Pos == _pos)
            return it;
    }

    return _squares.end();
}

int CChallenge_12::ComputeHeuristic_1(const Position& _pos) const
{
    return 60 * ((abs(m_EndPos.x - _pos.x) + abs(m_EndPos.y - _pos.y)) + 10 * ('z' - m_HeightMap[_pos.y][_pos.x]));
}


void CChallenge_12::Solve_2()
{
    m_SquaresToVisit.push_back(Square(m_EndPos, 0, ComputeHeuristic_2(m_EndPos)));

    while (!m_SquaresToVisit.empty())
    {
        sort(m_SquaresToVisit.begin(), m_SquaresToVisit.end());
        VisitNeighboors_2(*m_SquaresToVisit.begin());
    }
}


void CChallenge_12::VisitNeighboors_2(const Square& _square)
{
    Square currentSquare = _square;

    m_SquaresToVisit.erase(m_SquaresToVisit.begin());

    // Update Visited List
    {
        int distToEnd = GetDistToEnd(currentSquare.m_Pos);
        if (distToEnd != INT_MAX)
        {
            if (currentSquare.m_StepsTaken < distToEnd)
            {
                // Already visited with a higher cost, revisit
                SetDistToEnd(currentSquare.m_Pos, currentSquare.m_StepsTaken);
#ifdef _DEBUG
                m_DebugVisitedMap[currentSquare.m_Pos.y][currentSquare.m_Pos.x] = true;
#endif
            }
            else
            {
                // Already visited with a lower cost
                return;
            }
        }
        else
        {
            SetDistToEnd(currentSquare.m_Pos, currentSquare.m_StepsTaken);
#ifdef _DEBUG
            m_DebugVisitedMap[currentSquare.m_Pos.y][currentSquare.m_Pos.x] = true;
#endif
        }
    }

    // Update Best Start Point
    if (m_HeightMap[currentSquare.m_Pos.y][currentSquare.m_Pos.x] == 'a' && currentSquare.m_StepsTaken < GetDistToEnd(m_BestStartPoint))
    {
        m_BestStartPoint = currentSquare.m_Pos;
    }

    // Visit Neighboors
    vector<Position> neighboors;
    GetVisitableNeighboors_2(currentSquare.m_Pos, neighboors);
    for (const Position& neighboorPos : neighboors)
    {
        Square neighboorSquare(neighboorPos, currentSquare.m_StepsTaken + 1, ComputeHeuristic_2(neighboorPos));

        vector<Square>::iterator visitedIt = FindPos(m_VisitedSquares, neighboorSquare.m_Pos);
        if (visitedIt != m_VisitedSquares.end() && visitedIt->m_StepsTaken <= neighboorSquare.m_StepsTaken)
        {
            // Already visited with a lower or same cost
            continue;
        }

        vector<Square>::iterator toVisitIt = FindPos(m_SquaresToVisit, neighboorSquare.m_Pos);
        if (toVisitIt != m_SquaresToVisit.end())
        {
            if (neighboorSquare.m_StepsTaken < toVisitIt->m_StepsTaken)
            {
                // Already planned to visit it with a higher cost, replace cost
                m_SquaresToVisit.erase(toVisitIt);
                m_SquaresToVisit.push_back(neighboorSquare);
            }
            else
            {
                // Already planned to visit with a lower cost
                continue;
            }
        }
        else
        {
            m_SquaresToVisit.push_back(neighboorSquare);
        }
    }
}


void CChallenge_12::GetVisitableNeighboors_2(const Position& _pos, vector<Position>& _outNeighboors)
{
    const char& currentHeight = m_HeightMap[_pos.y][_pos.x];

    for (int yOffset = -1; yOffset <= 1; yOffset += 2)
    {
        int yPos = _pos.y + yOffset;
        if (yPos < 0 || yPos > m_HeightMap.size() - 1)
            continue;

        Position pos(_pos.x, yPos);
        const char& height = m_HeightMap[yPos][_pos.x];
        if (currentHeight < height || currentHeight - height  < 2)
        {
            _outNeighboors.push_back(pos);
        }
    }

    const vector<char>& row = m_HeightMap[_pos.y];
    for (int xOffset = -1; xOffset <= 1; xOffset += 2)
    {
        int xPos = _pos.x + xOffset;
        if (xPos < 0 || xPos > row.size() - 1)
            continue;

        Position pos(xPos, _pos.y);
        const char& height = row[xPos];
        if (currentHeight < height || currentHeight - height < 2)
        {
            _outNeighboors.push_back(pos);
        }
    }
}


int CChallenge_12::ComputeHeuristic_2(const Position& _pos) const
{
    return 60 * ((abs(m_EndPos.x - _pos.x) + abs(m_EndPos.y - _pos.y)) + 10 * (m_HeightMap[_pos.y][_pos.x] - 'a'));
}


int CChallenge_12::GetDistToEnd(const Position& _pos) const
{
    return m_DistToEndMap[_pos.y][_pos.x];
}


void CChallenge_12::SetDistToEnd(const Position& _pos, int _value)
{
    m_DistToEndMap[_pos.y][_pos.x] = _value;
}

#ifdef _DEBUG
void CChallenge_12::PrintVisitedMap() const
{
    int visitedPosCount = 0;

    cout << endl;
    for (int i = 0; i < m_DebugVisitedMap.size(); ++i)
    {
        const vector<bool>& row = m_DebugVisitedMap[i];
        for (int j = 0; j < row.size(); ++j)
        {
            if (i == m_StartPos.y && j == m_StartPos.x)
                cout << "S";
            else if (i == m_EndPos.y && j == m_EndPos.x)
                cout << "E";
            else
                cout << (row[j] ? 'o' : '.');

            visitedPosCount += (row[j] ? 1 : 0);
        }

        cout << endl;
    }

    cout << visitedPosCount << " squares were visited." << endl;
}
#endif
