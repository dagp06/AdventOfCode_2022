#include "Challenge_16.h"
#include "../Helpers/FileHelpers.h"

#include <cassert>



////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_16::sm_inputFilePath = "Inputs/Input_Challenge_16.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_16::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    for (const string& line : lines)
    {
        ValveID id = line.substr(6, 2);
        Valve& newValve = m_Valves[id];

        vector<string> valveStrings;
        FileHelper::SplitLine(line, ";", valveStrings);

        vector<string> valveFlowRateStrings;
        FileHelper::SplitLine(valveStrings[0], "=", valveFlowRateStrings);
        newValve.m_FlowRate = stoi(valveFlowRateStrings[1]);

        vector<string> neigboorValveStrings;
        FileHelper::SplitLine(valveStrings[1], ", ", neigboorValveStrings);
        for (const string& neighboorValve : neigboorValveStrings)
        {
            newValve.m_NeighboorValves.push_back(neighboorValve.substr(neighboorValve.size() - 2));
        }
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_16::Run_FirstPart()
{
    FillValveDistanceMaps();


    Simulate("AA", 30);

    int mostPressureReleased = m_BestState.m_TotalPressureReleased;

#ifndef BENCHMARK
    cout << "The most pressure that can be released is " << mostPressureReleased << "." << endl;
#endif // !BENCHMARK

    return EErrorCode::Success;
}

EErrorCode CChallenge_16::CleanUp_FirstPart()
{
    return EErrorCode::NotImplemented;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_16::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }
    
    return EErrorCode::NotImplemented;
}

EErrorCode CChallenge_16::Run_SecondPart()
{
    return EErrorCode::NotImplemented;
}

EErrorCode CChallenge_16::CleanUp_SecondPart()
{
    return EErrorCode::NotImplemented;
}

void CChallenge_16::FillValveDistanceMaps()
{
    for (map<ValveID, Valve>::iterator valveIt = m_Valves.begin(); valveIt != m_Valves.end(); ++valveIt)
    {
        ValveID sourceValveID = valveIt->first;

        map<ValveID, int>& sourceValveDistanceMap = m_ValveDistanceMaps[sourceValveID];

        set<ValveID> processedValves;

        for (map<ValveID, Valve>::iterator otherValveIt = m_Valves.begin(); otherValveIt != m_Valves.end(); ++otherValveIt)
        {
            if (valveIt == otherValveIt)
                sourceValveDistanceMap[sourceValveID] = 0;
            else
                sourceValveDistanceMap[otherValveIt->first] = INT_MAX;
        }

        ValveID valveIDToProcess;
        while (processedValves.size() < m_Valves.size())
        {
            // Pick the valve with a minimum distance value
            int minDist = INT_MAX;
            for (map<ValveID, int>::const_iterator it = sourceValveDistanceMap.begin(); it != sourceValveDistanceMap.end(); ++it)
            {
                if (processedValves.find(it->first) != processedValves.end())
                    continue;

                if (it->second < minDist)
                {
                    minDist = it->second;
                    valveIDToProcess = it->first;
                }
            }

            // Include it in processedValves
            processedValves.insert(valveIDToProcess);

            // Update distance values of its neighboors
            int newNeighboorDist = minDist + 1;
            const vector<ValveID>& neighboorValves = m_Valves[valveIDToProcess].m_NeighboorValves;
            for (int i = 0; i < neighboorValves.size(); ++i)
            {
                int& neighboorDist = sourceValveDistanceMap[neighboorValves[i]];
                neighboorDist = min(neighboorDist, newNeighboorDist);
            }
        }
    }
}


void CChallenge_16::Simulate(const ValveID& _startValveID, int _duration)
{
    State* initialState = new State();
    for (map<ValveID, Valve>::const_iterator it = m_Valves.begin(); it != m_Valves.end(); ++it)
    {
        if (it->second.m_FlowRate > 0)
            initialState->m_ValvesToOpen.push_back(it->first);
    }
    initialState->m_CurrentValve = _startValveID;
    initialState->m_RemainingDuration = _duration;

    m_States.push_back(initialState);

    while (!m_States.empty())
    {
        TickStates();
    }
}


void CChallenge_16::TickStates()
{
    vector<State*> newStates;

    for (vector<State*>::iterator stateIt = m_States.begin(); stateIt != m_States.end();)
    {
        State& currentState = **stateIt;

        // If there are valves to open
        vector<ValveID>& valvesToOpen = currentState.m_ValvesToOpen;
        if (!valvesToOpen.empty())
        {
            // Create a new state for each closed valve
            for (int i = 0; i < valvesToOpen.size(); ++i)
            {
                const ValveID destValveID = valvesToOpen[i];

//                 State* newState;
//                 if (i == valvesToOpen.size() - 1)
//                 {
//                     newState = &currentState;
//                 }
//                 else
//                 {
//                     newState = new State(currentState);
//                     newStates.push_back(newState);
//                 }
				State* newState = new State(currentState);
				newStates.push_back(newState);

                int timeToOpenValve = m_ValveDistanceMaps[currentState.m_CurrentValve][destValveID] + 1;

                if (newState->Tick(timeToOpenValve))
                {
                    newState->m_CurrentFlowRate += m_Valves[destValveID].m_FlowRate;
                    newState->m_CurrentValve = destValveID;

                    newState->m_ValvesToOpen.erase(find(newState->m_ValvesToOpen.begin(), newState->m_ValvesToOpen.end(), destValveID));

                    newState->m_OpenedValves.push_back(destValveID);
                }
                else
                {
                    // Couldn't reach target valve in time. Stop here.
                    newState->m_ValvesToOpen.clear();
                }
            }

            // ++stateIt;

			delete* stateIt;
			stateIt = m_States.erase(stateIt);
        }
        else // All valves are opened already
        {
            currentState.Tick(currentState.m_RemainingDuration);
            if (m_BestState.m_TotalPressureReleased < currentState.m_TotalPressureReleased)
            {
                m_BestState = currentState;
            }

            delete *stateIt;
            stateIt = m_States.erase(stateIt);
        }
    }

    m_States.insert(m_States.end(), newStates.begin(), newStates.end());
}

bool CChallenge_16::State::Tick(int _delta /*= 1*/)
{
    bool result = true;
    if (_delta > m_RemainingDuration)
    {
        result = false;
        _delta = m_RemainingDuration;
    }

    m_TotalPressureReleased += _delta * m_CurrentFlowRate;
    m_RemainingDuration -= _delta;

    return result;
}
