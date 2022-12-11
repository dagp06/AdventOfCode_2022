#include "Challenge_03.h"
#include "../Helpers/FileHelpers.h"

#include <string.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_03::sm_inputFilePath = "Inputs/Input_Challenge_03.txt";


////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_03::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    for (const string& line : lines)
    {
        m_AllRucksackContent.push_back(line);
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_03::Run_FirstPart()
{
    int prioritiesSum = 0;
    for (const string& rucksackContent : m_AllRucksackContent)
    {
        char duplicateItem = (char)0;

        const size_t halfLineIndex = rucksackContent.size() / 2;
        const string firstCompartment = rucksackContent.substr(0, halfLineIndex);
        const string secondCompartment = rucksackContent.substr(halfLineIndex, halfLineIndex);
        for (const char& item : firstCompartment)
        {
            if (secondCompartment.find(item) != string::npos)
            {
                duplicateItem = item;
                break;
            }
        }

        prioritiesSum += ComputeItemPriority(duplicateItem);
    }

    cout << "The sum of the priorities is " << prioritiesSum << "." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_03::CleanUp_FirstPart()
{
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_03::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    for (const string& line : lines)
    {
        m_AllRucksackContent.push_back(line);
    }
    
    return EErrorCode::Success;
}

EErrorCode CChallenge_03::Run_SecondPart()
{
    const int elvesPerGroup = 3;
    int prioritiesSum = 0;
    for (int i = 0; i < m_AllRucksackContent.size(); i += 3)
    {
        int smallestContentIndex = i;
        for (int j = 1; j < elvesPerGroup; ++j)
        {
            int index = i + j;
            if (m_AllRucksackContent[index].size() < m_AllRucksackContent[smallestContentIndex].size())
            {
                smallestContentIndex = index;
            }
        }

        const string& smallesContentInGroup = m_AllRucksackContent[smallestContentIndex];
        for (const char& item : smallesContentInGroup)
        {
            bool itemNotFound = false;
            for (int j = 0; j < elvesPerGroup; ++j)
            {
                int index = i + j;
                if (index == smallestContentIndex)
                    continue;

                if (m_AllRucksackContent[index].find(item) == string::npos)
                {
                    itemNotFound = true;
                    break;
                }
            }

            if (itemNotFound)
                continue;

            prioritiesSum += ComputeItemPriority(item);
            break;
        }
    }

    cout << "The sum of the priorities is " << prioritiesSum << "." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_03::CleanUp_SecondPart()
{
    return EErrorCode::Success;
}


int CChallenge_03::ComputeItemPriority(char _item)
{
    int priority = 0;
    if (_item > 'Z')
    {
        priority = _item - 'a' + 1;
    }
    else
    {
        priority = _item - 'A' + 27;
    }

    return priority;
}