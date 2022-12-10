#include "Challenge.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge::SetUp(bool const isUsingFirstPartCode)
{
    m_isUsingFirstPartCode = isUsingFirstPartCode;

    return isUsingFirstPartCode ? SetUp_FirstPart() : SetUp_SecondPart();
}

EErrorCode CChallenge::Run()
{
    return m_isUsingFirstPartCode ? Run_FirstPart() : Run_SecondPart();
}

EErrorCode CChallenge::CleanUp()
{
    return m_isUsingFirstPartCode ? CleanUp_FirstPart() : CleanUp_SecondPart();
}
