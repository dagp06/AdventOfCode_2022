#include "Challenge_15.h"
#include "../Helpers/FileHelpers.h"


////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_15::sm_inputFilePath = "Inputs/Input_Challenge_15.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_15::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    for (const string& line : lines)
    {
        vector<string> positionStrings;
        FileHelper::SplitLine(line, ":", positionStrings);

        // Sensor
        positionStrings[0].erase(0, 10);
        vector<string> sensorStrings;
        FileHelper::SplitLine(positionStrings[0], ", ", sensorStrings);
        sensorStrings[0].erase(0, 2);
        sensorStrings[1].erase(0, 2);
        Position sensorPos(stoi(sensorStrings[0]), stoi(sensorStrings[1]));

        // Beacon
        positionStrings[1].erase(0, 22);
        vector<string> beaconStrings;
        FileHelper::SplitLine(positionStrings[1], ", ", beaconStrings);
        beaconStrings[0].erase(0, 2);
        beaconStrings[1].erase(0, 2);
        Position beaconPos(stoi(beaconStrings[0]), stoi(beaconStrings[1]));

        m_SensorsBeacon[sensorPos] = beaconPos;
        m_Beacons.insert(beaconPos);
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_15::Run_FirstPart()
{
    int rowToCheck = 2000000;
    RangeList beaconFreeRangesOnRow;

    set<int> beaconPosOnRow;
    for (set<Position>::const_iterator it = m_Beacons.begin(); it != m_Beacons.end(); ++it)
    {
        if (it->y == rowToCheck)
        {
            beaconPosOnRow.insert(it->x);
        }
    }

#ifdef _DEBUG
    int i = 1;
#endif // _DEBUG

    for (map<Position, Position>::const_iterator it = m_SensorsBeacon.begin(); it != m_SensorsBeacon.end(); ++it)
    {
#ifdef _DEBUG
        cout << "Processing " << i++ << "/" << m_SensorsBeacon.size() << "..." << endl;
#endif // _DEBUG

        const Position& sensorPos = it->first;
        const Position& beaconPos = it->second;

        int sensorToRow = abs(rowToCheck - sensorPos.y);
        int beaconRadius = it->first.ComputeManhattanDistance(beaconPos);
        if (sensorToRow <= beaconRadius)
        {
            int halfOfFreePosCount = beaconRadius - sensorToRow;
            int startPos = sensorPos.x - halfOfFreePosCount;
            int endPos = sensorPos.x + halfOfFreePosCount;
            vector<Range> rangesToAdd;
            for (set<int>::const_iterator beaconPosIt = beaconPosOnRow.begin(); beaconPosIt != beaconPosOnRow.end(); ++beaconPosIt)
            {
                if (*beaconPosIt > endPos)
                    break;

                if (*beaconPosIt < startPos)
                    continue;

                // Beacon is in current range
                // Insert a range from start pos to before the beacon
                if (*beaconPosIt > startPos)
                {
                    rangesToAdd.push_back(Range(startPos, *beaconPosIt - 1));
                }

                startPos = *beaconPosIt + 1;
            }

            if (startPos <= endPos)
            {
                // Add last range
                rangesToAdd.push_back(Range(startPos, endPos));
            }

            for (vector<Range>::const_iterator rangeIt = rangesToAdd.begin(); rangeIt != rangesToAdd.end(); ++rangeIt)
            {
                beaconFreeRangesOnRow.AddRange(*rangeIt);
            }
        }
    }

    int beaconFreePositionCount = beaconFreeRangesOnRow.ComputeTotalSize();

#ifndef BENCHMARK
    cout << "At row y=" << rowToCheck << ", there are " << beaconFreePositionCount << " positions that cannot contain a beacon." << endl;
#endif // !BENCHMARK

    return EErrorCode::Success;
}

EErrorCode CChallenge_15::CleanUp_FirstPart()
{
    m_SensorsBeacon.clear();
    m_Beacons.clear();
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_15::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    for (const string& line : lines)
    {
        vector<string> positionStrings;
        FileHelper::SplitLine(line, ":", positionStrings);

        // Sensor
        positionStrings[0].erase(0, 10);
        vector<string> sensorStrings;
        FileHelper::SplitLine(positionStrings[0], ", ", sensorStrings);
        sensorStrings[0].erase(0, 2);
        sensorStrings[1].erase(0, 2);
        Position sensorPos(stoi(sensorStrings[0]), stoi(sensorStrings[1]));

        // Beacon
        positionStrings[1].erase(0, 22);
        vector<string> beaconStrings;
        FileHelper::SplitLine(positionStrings[1], ", ", beaconStrings);
        beaconStrings[0].erase(0, 2);
        beaconStrings[1].erase(0, 2);
        Position beaconPos(stoi(beaconStrings[0]), stoi(beaconStrings[1]));

        m_SensorsBeacon[sensorPos] = beaconPos;
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_15::Run_SecondPart()
{
    const int areaSize = 4000000;
    m_SensorCoveredRanges.resize(areaSize);

#ifdef _DEBUG
    int i = 1;
#endif // _DEBUG

    for (map<Position, Position>::const_iterator it = m_SensorsBeacon.begin(); it != m_SensorsBeacon.end(); ++it)
    {
#ifdef _DEBUG
        cout << "Processing " << i++ << "/" << m_SensorsBeacon.size() << "..." << endl;
#endif // _DEBUG
        const Position& sensorPos = it->first;
        const Position& beaconPos = it->second;

        int rangeRadius = sensorPos.ComputeManhattanDistance(beaconPos);

        for (int i = max(sensorPos.y - rangeRadius, 0); i < min(sensorPos.y + rangeRadius, areaSize); ++i)
        {
            RangeList& rangeList = m_SensorCoveredRanges[i];

            int halfRangeSize = rangeRadius - abs(sensorPos.y - i);
            rangeList.AddRange(Range(max(sensorPos.x - halfRangeSize, 0), min(sensorPos.x + halfRangeSize, areaSize)));
        }
    }

    Position distressSignalPos;
    for (int i = 0; i < areaSize; ++i)
    {
        if (m_SensorCoveredRanges[i].ComputeTotalSize() < areaSize + 1)
        {
            distressSignalPos.y = i;
            if (m_SensorCoveredRanges[i].GetRangeCount() == 1)
            {
                // Distress signal is on an edge of the area
                const set<Range>& ranges = m_SensorCoveredRanges[i].GetRanges();
                set<Range>::const_iterator rangeIt = ranges.begin();
                if (rangeIt != ranges.end())
                {
                    if (rangeIt->m_Start == 0)
                    {
                        distressSignalPos.x = rangeIt->m_End + 1;
                    }
                    else
                    {
                        distressSignalPos.x = rangeIt->m_Start - 1;
                    }
                }
            }
            else // Range count == 2
            {
                // Distress signal is between the two ranges
                const set<Range>& ranges = m_SensorCoveredRanges[i].GetRanges();
                set<Range>::const_iterator firstRangeIt = ranges.begin();
                if (firstRangeIt != ranges.end())
                {
                    distressSignalPos.x = firstRangeIt->m_End + 1;
                }
            }
            break;
        }
    }

#ifdef _DEBUG
    cout << "The distress signal's position is (" << distressSignalPos.x << ", " << distressSignalPos.y << ")." << endl;
#endif // _DEBUG

    long long tuningFrequency = distressSignalPos.x * (long long)areaSize + distressSignalPos.y;
#ifndef BENCHMARK
    cout << "The tuning frequency is " << tuningFrequency << "." << endl;
#endif // !BENCHMARK

    return EErrorCode::Success;
}

EErrorCode CChallenge_15::CleanUp_SecondPart()
{
    m_SensorsBeacon.clear();
    m_SensorCoveredRanges.clear();
    return EErrorCode::Success;
}

bool CChallenge_15::Range::CanMergeWith(const Range& _other) const
{
    return !((m_End < _other.m_Start - 1) || (m_Start + 1 > _other.m_End));
}

void CChallenge_15::Range::MergeWith(const Range& _other)
{
    m_Start = min(m_Start, _other.m_Start);
    m_End = max(m_End, _other.m_End);
}

void CChallenge_15::RangeList::AddRange(const Range& _range)
{
    bool merged = false;
    for (set<Range>::iterator it = m_Ranges.begin(); it != m_Ranges.end(); ++it)
    {
        if (it->CanMergeWith(_range))
        {
            if (!it->FullyContains(_range))
            {
                Range newRange(*it);
                newRange.MergeWith(_range);
                m_Ranges.erase(it++);

                // Merge all following overlapping ranges too
                while (it != m_Ranges.end())
                {
                    if (it->CanMergeWith(newRange))
                    {
                        newRange.MergeWith(*it);
                        m_Ranges.erase(it++);
                    }
                    else
                    {
                        break;
                    }
                }

                m_Ranges.insert(newRange);
            }

            merged = true;
            break;
        }
    }

    if (!merged)
    {
        m_Ranges.insert(_range);
    }
}

int CChallenge_15::RangeList::ComputeTotalSize() const
{
    int totalSize = 0;
    for (set<Range>::const_iterator it = m_Ranges.begin(); it != m_Ranges.end(); ++it)
    {
        totalSize += it->ComputeSize();
    }

    return totalSize;
}
