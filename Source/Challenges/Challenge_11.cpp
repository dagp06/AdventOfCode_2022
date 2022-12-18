#include "Challenge_11.h"
#include "../Helpers/FileHelpers.h"

#include <set>

////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_11::sm_inputFilePath = "Inputs/Input_Challenge_11.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_11::SetUp_FirstPart()
{
    return SetUp_Internal();
}

EErrorCode CChallenge_11::Run_FirstPart()
{
    const int roundCount = 20;
    for (int i = 0; i < roundCount; ++i)
    {
        for (map<int, Monkey>::iterator it = m_Monkeys.begin(); it != m_Monkeys.end(); ++it)
        {
            Monkey& monkey = it->second;

            Monkey::ItemThrowAction throwAction;
            while (monkey.InspectNextItem(throwAction, true))
            {
                m_Monkeys[throwAction.second].CatchItem(throwAction.first);
            }
        }
    }

    int monkeyBusinessLevel = 1;

    set<int> inspectionCounts;
    for (map<int, Monkey>::iterator it = m_Monkeys.begin(); it != m_Monkeys.end(); ++it)
    {
        Monkey& monkey = it->second;
        inspectionCounts.insert(monkey.GetInspectionsDone());
    }

    int inspectionCountProcessed = 0;
    for (set<int>::const_reverse_iterator it = inspectionCounts.rbegin(); it != inspectionCounts.rend() && inspectionCountProcessed < m_MostActiveMonkeyCount; ++it)
    {
        monkeyBusinessLevel *= *it;
        ++inspectionCountProcessed;
    }

#ifndef BENCHMARK
    cout << "The level of monkey business is " << monkeyBusinessLevel << endl;
#endif // BENCHBARK

    return EErrorCode::Success;
}

EErrorCode CChallenge_11::CleanUp_FirstPart()
{
    m_Monkeys.clear();
    m_DivisorProduct = 1;
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_11::SetUp_SecondPart()
{
    return SetUp_Internal();
}

EErrorCode CChallenge_11::Run_SecondPart()
{
    const int roundCount = 10000;

    for (int i = 0; i < roundCount; )
    {
        for (map<int, Monkey>::iterator it = m_Monkeys.begin(); it != m_Monkeys.end(); ++it)
        {
            Monkey& monkey = it->second;

            Monkey::ItemThrowAction throwAction;
            while (monkey.InspectNextItem(throwAction, false))
            {
                m_Monkeys[throwAction.second].CatchItem(throwAction.first % m_DivisorProduct);
            }
        }

        ++i;
    }

    long long monkeyBusinessLevel = 1;

    set<int> inspectionCounts;
    for (map<int, Monkey>::iterator it = m_Monkeys.begin(); it != m_Monkeys.end(); ++it)
    {
        Monkey& monkey = it->second;
        inspectionCounts.insert(monkey.GetInspectionsDone());
    }

    int inspectionCountProcessed = 0;
    for (set<int>::const_reverse_iterator it = inspectionCounts.rbegin(); it != inspectionCounts.rend() && inspectionCountProcessed < m_MostActiveMonkeyCount; ++it)
    {
        monkeyBusinessLevel *= *it;
        ++inspectionCountProcessed;
    }

#ifndef BENCHMARK
    cout << "The level of monkey business is " << monkeyBusinessLevel << endl;
#endif // BENCHBARK

    return EErrorCode::Success;
}

EErrorCode CChallenge_11::CleanUp_SecondPart()
{
    m_Monkeys.clear();
    m_DivisorProduct = 1;
    return EErrorCode::Success;
}


EErrorCode CChallenge_11::SetUp_Internal()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    m_MostActiveMonkeyCount = 2;
    m_DivisorProduct = 1;

    int monkeyCount = 0;
    for (int i = 0; i < lines.size(); i += 7)
    {
        Monkey& monkey = m_Monkeys[monkeyCount];

        // Item Worry List
        string& worryLine = lines[i + 1];
        worryLine.erase(0, worryLine.find(":") + 2);

        vector<string> itemWorryLevelsStr;
        FileHelper::SplitLine(worryLine, ", ", itemWorryLevelsStr);
        for (const string& worryLevelStr : itemWorryLevelsStr)
        {
            monkey.CatchItem(stoi(worryLevelStr));
        }


        // Operation
        string& operationLine = lines[i + 2];
        operationLine.erase(0, operationLine.find("=") + 2);
        vector<string> operationStr;
        FileHelper::SplitLine(operationLine, " ", operationStr);

        int value = 0;
        bool isOperandOld = true;
        if (isdigit(operationStr[2][0]))
        {
            value = stoi(operationStr[2]);
            isOperandOld = false;
        }

        Operator op = operationStr[1][0] == '+' ? Operator::Addition : Operator::Multiplication;

        monkey.SetOperation(op, isOperandOld, value);

        // Divisible Test
        string& divisorValueLine = lines[i + 3];
        divisorValueLine.erase(0, divisorValueLine.find("by") + 3);
        int divisorValue = stoi(divisorValueLine);
        m_DivisorProduct *= divisorValue;

        string& testTrueIDLine = lines[i + 4];
        testTrueIDLine.erase(0, testTrueIDLine.find("ey") + 3);
        int targetTrue = stoi(testTrueIDLine);

        string& testFalseIDLine = lines[i + 5];
        testFalseIDLine.erase(0, testFalseIDLine.find("ey") + 3);
        int targetFalse = stoi(testFalseIDLine);
        monkey.SetDivisibleTest(divisorValue, targetTrue, targetFalse);

        ++monkeyCount;
    }

    return EErrorCode::Success;
}

void CChallenge_11::PrintMonkeyInspectionCounts(int _roundCompleted) const
{
    cout << "== After round " << _roundCompleted << " ==" << endl;

    for (map<int, Monkey>::const_iterator it = m_Monkeys.begin(); it != m_Monkeys.end(); ++it)
    {
        const Monkey& monkey = it->second;
        cout << "Monkey " << it->first << " inspected items " << monkey.GetInspectionsDone() << " times." << endl;
    }

    cout << endl;
}

long long CChallenge_11::Operation::Compute(const long long& _old) const
{
    switch (m_Operator)
    {
    case CChallenge_11::Operator::Addition:
        return _old + (m_IsOperandOld ? _old : m_Value);

    case CChallenge_11::Operator::Multiplication:
        return _old * (m_IsOperandOld ? _old : m_Value);

    default:
        break;
    }

    return 0;
}

int CChallenge_11::DivisibleTest::Run(const long long& _worryValue) const
{
    return _worryValue % m_Divisor == 0 ? m_TargetTrue : m_TargetIfFalse;
}

void CChallenge_11::Monkey::SetOperation(Operator _op, bool _isOperandOld, int _value)
{
    m_Operation = Operation(_op, _isOperandOld, _value);
}

void CChallenge_11::Monkey::SetDivisibleTest(int _divisor, int _targetTrue, int _targetFalse)
{
    m_Test = DivisibleTest(_divisor, _targetTrue, _targetFalse);
}

bool CChallenge_11::Monkey::InspectNextItem(ItemThrowAction& _outItemThrowAction, bool _withRelief)
{
    if (m_CurrentItemWorryLevels.empty())
        return false;

    long long worryLevel = m_CurrentItemWorryLevels.front();
    m_CurrentItemWorryLevels.pop();

    worryLevel = m_Operation.Compute(worryLevel);
    if (_withRelief)
        worryLevel /= 3;
    
    _outItemThrowAction.first = worryLevel;
    _outItemThrowAction.second = m_Test.Run(worryLevel);

    ++m_InspectionsDone;

    return true;
}
