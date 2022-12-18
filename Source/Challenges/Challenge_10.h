#pragma once

#include "Common/Challenge.h"

#include <map>

using namespace std;

class CChallenge_10 : public CChallenge
{
private:
    enum class OperationType
    {
        noop,
        addx
    };

    struct Operation
    {
        Operation(const OperationType _operationType) : m_OperationType(_operationType), m_Operand(0) {}
        Operation(const OperationType _operationType, const int _operand) : m_OperationType(_operationType), m_Operand(_operand) {}

        OperationType m_OperationType;
        int m_Operand;
    };

    virtual EErrorCode SetUp_FirstPart() override;
    virtual EErrorCode Run_FirstPart() override;
    virtual EErrorCode CleanUp_FirstPart() override;

    virtual EErrorCode SetUp_SecondPart() override;
    virtual EErrorCode Run_SecondPart() override;
    virtual EErrorCode CleanUp_SecondPart() override;

    void ProcessOperation_1(const Operation& _operation, int& _currentCycle, int& _x);

    void ProcessOperation_2(const Operation& _operation, int& _currentCycle, int& _x);

    static string const sm_inputFilePath;

    map<OperationType, int> m_OperationDuration;
    vector<Operation> m_Operations;

    int m_CyclePerLine;
};
