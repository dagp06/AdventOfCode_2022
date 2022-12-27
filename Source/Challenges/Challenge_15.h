#pragma once

#include "Common/Challenge.h"

#include <map>
#include <set>

using namespace std;

class CChallenge_15 : public CChallenge
{
private:
    struct Range
    {
        Range(int _start, int _end) : m_Start(_start), m_End(_end) {}
        Range(const Range& _other) : m_Start(_other.m_Start), m_End(_other.m_End) {}

        int ComputeSize() const { return m_End - m_Start + 1; }

        bool FullyContains(const Range& _other) const { return _other.m_Start >= m_Start && _other.m_End <= m_End; }
        bool CanMergeWith(const Range& _other) const;
        void MergeWith(const Range& _other);

        bool operator<(const Range& _other) const { return m_Start < _other.m_Start || m_Start == _other.m_Start && m_End < _other.m_End; }

        int m_Start = 0;
        int m_End = INT_MAX;
    };

    class RangeList
    {
    public:
        const set<Range>& GetRanges() const { return m_Ranges; }
        int GetRangeCount() const { return (int)m_Ranges.size(); }

        void AddRange(const Range& _range);
        int ComputeTotalSize() const;

    private:
        set<Range> m_Ranges;
    };

    struct Position
    {
        Position() {}
        Position(int _x, int _y) : x(_x), y(_y) {}

        int ComputeManhattanDistance(const Position& _other) const { return abs(x - _other.x) + abs(y - _other.y); }

        bool operator<(const Position& _other) const { return x < _other.x || (x == _other.x && y < _other.y); }
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

    static string const sm_inputFilePath;

    // Key: Sensor's Pos / Value: Detected Beacon's Pos
    map<Position, Position> m_SensorsBeacon;
    set<Position> m_Beacons;

    vector<RangeList> m_SensorCoveredRanges;
};
