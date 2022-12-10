#pragma once

#include "../../CommonDefinitions.h"

class CChallenge
{
public:
    EErrorCode SetUp(bool const isUsingFirstPartCode);
    EErrorCode Run();
    EErrorCode CleanUp();

    bool IsUsingFirstPartCode() const { return m_isUsingFirstPartCode; }

private:
    virtual EErrorCode SetUp_FirstPart() = 0;
    virtual EErrorCode Run_FirstPart() = 0;
    virtual EErrorCode CleanUp_FirstPart() = 0;

    virtual EErrorCode SetUp_SecondPart() = 0;
    virtual EErrorCode Run_SecondPart() = 0;
    virtual EErrorCode CleanUp_SecondPart() = 0;

    bool m_isUsingFirstPartCode = true;
};
