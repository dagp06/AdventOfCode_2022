#pragma once

#include "Common/Challenge.h"

#include <map>
#include <set>

using namespace std;

class CChallenge_16 : public CChallenge
{
private:
    typedef string ValveID;

    struct Valve
    {
        int m_FlowRate = 0;
        vector<ValveID> m_NeighboorValves;
    };

    struct State
    {
        bool Tick(int _delta = 1);

        ValveID m_CurrentValve;
        int m_RemainingDuration = 0;
        int m_CurrentFlowRate = 0;
        int m_TotalPressureReleased = 0;
        vector<ValveID> m_ValvesToOpen;
        vector<ValveID> m_OpenedValves;
    };

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    void FillValveDistanceMaps();
    void Simulate(const ValveID& _startValveID, int _duration);
    void TickStates();

    static string const sm_inputFilePath;

    map<ValveID, Valve> m_Valves;
    map<ValveID, map<ValveID, int>> m_ValveDistanceMaps;

    vector<State*> m_States;
    State m_BestState;
};
