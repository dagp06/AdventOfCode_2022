#pragma once

#include "Common/Challenge.h"

#include <map>
#include <queue>

using namespace std;

class CChallenge_11 : public CChallenge
{
private:

    enum class Operator
    {
        Addition,
        Multiplication
    };

    struct Operation
    {
        Operation() {}
        Operation(Operator _op, bool _isOperandOld, int _value = 0) : m_Operator(_op), m_IsOperandOld(_isOperandOld), m_Value(_value) {}

        long long Compute(const long long& _old) const;

        bool m_IsOperandOld = false;
        int m_Value = 0;
        Operator m_Operator = Operator::Addition;
    };

    class DivisibleTest
    {
    public:
        DivisibleTest() {}
        DivisibleTest(int _divisor, int _targetIfTrue, int _targetFalse) : m_Divisor(_divisor), m_TargetTrue(_targetIfTrue), m_TargetIfFalse(_targetFalse) {}

        int Run(const long long& _worryValue) const;

    private:
        int m_Divisor = 0;
        int m_TargetTrue = 0;
        int m_TargetIfFalse = 0;
    };

    class Monkey
    {
    public:
        // Key : Item Worry Level - Value : Dest Monkey ID
        typedef pair<long long, int> ItemThrowAction;

        queue<long long>& GetCurrentItemWorryLevels() { return m_CurrentItemWorryLevels; }
        int GetInspectionsDone() const { return m_InspectionsDone; }

        void SetOperation(Operator _op, bool _isOperandOld, int _value);
        void SetDivisibleTest(int _divisor, int _targetTrue, int _targetFalse);

        bool InspectNextItem(ItemThrowAction& _outItemThrowAction, bool _withRelief);
        void CatchItem(long long _itemWorryLevel) { m_CurrentItemWorryLevels.push(_itemWorryLevel); }

    private:
        long long ComputeOperation(long long& m_Old);

        queue<long long> m_CurrentItemWorryLevels;
        Operation m_Operation;
        DivisibleTest m_Test;
        int m_InspectionsDone = 0;
    };

    struct MonkeySorter
    {
        bool operator()(const Monkey& a, const Monkey& b)
        {
            return a.GetInspectionsDone() > b.GetInspectionsDone();
        }
    };

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    void PrintMonkeyInspectionCounts(int _roundCompleted) const;

    static string const sm_inputFilePath;

private:
    EErrorCode SetUp_Internal();

    map<int, Monkey> m_Monkeys;

    int m_MostActiveMonkeyCount;

    long long m_DivisorProduct = 1;
};
