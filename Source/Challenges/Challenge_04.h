#pragma once

#include "Common/Challenge.h"

using namespace std;

class CChallenge_04 : public CChallenge
{
private:
    class SectionRange
    {
    public:
        SectionRange(int _min, int _max) : m_Min(_min), m_Max(_max) {}

        bool Contains(int _section) const { return _section >= m_Min && _section <= m_Max; }
        bool FullyContains(const SectionRange& _sectionRange) const { return _sectionRange.m_Min >= m_Min && _sectionRange.m_Max <= m_Max; }
        bool Overlaps(const SectionRange& _sectionRange) const { return Contains(_sectionRange.m_Min) || _sectionRange.Contains(m_Min); }

    private:
        int m_Min = 0;
        int m_Max = 0;
    };

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    static string const sm_inputFilePath;

    typedef pair<SectionRange, SectionRange> SectionRangePair;
    vector<SectionRangePair> m_SectionRangePairs;
};
