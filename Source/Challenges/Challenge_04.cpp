#include "Challenge_04.h"
#include "../Helpers/FileHelpers.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
string const CChallenge_04::sm_inputFilePath = "Inputs/Input_Challenge_04.txt";



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_04::SetUp_FirstPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
    }

    for (const string& line : lines)
    {
        vector<string> sectionRangePairs;
        FileHelper::SplitLine(line, ",", sectionRangePairs);

        vector<string> firstSectionRangeNumbers;
        FileHelper::SplitLine(sectionRangePairs[0], "-", firstSectionRangeNumbers);
        SectionRange firstRange = SectionRange(stoi(firstSectionRangeNumbers[0]), stoi(firstSectionRangeNumbers[1]));


		vector<string> secondSectionRangeNumbers;
		FileHelper::SplitLine(sectionRangePairs[1], "-", secondSectionRangeNumbers);
		SectionRange secondRange = SectionRange(stoi(secondSectionRangeNumbers[0]), stoi(secondSectionRangeNumbers[1]));

        m_SectionRangePairs.push_back(SectionRangePair(firstRange, secondRange));
    }

    return EErrorCode::Success;
}

EErrorCode CChallenge_04::Run_FirstPart()
{
    int numberOfPairsToReconsider = 0;
    for (const SectionRangePair& sectionRangePair : m_SectionRangePairs)
    {
        if (sectionRangePair.first.FullyContains(sectionRangePair.second) || sectionRangePair.second.FullyContains(sectionRangePair.first))
            ++numberOfPairsToReconsider;
    }

    cout << "There are " << numberOfPairsToReconsider << " pairs where one range fully contains the other." << endl;

    return EErrorCode::Success;
}

EErrorCode CChallenge_04::CleanUp_FirstPart()
{
    return EErrorCode::Success;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
EErrorCode CChallenge_04::SetUp_SecondPart()
{
    vector<string> lines;
    EErrorCode const readErrorCode = FileHelper::ReadLines(sm_inputFilePath, lines);
    if (readErrorCode != EErrorCode::Success)
    {
        return readErrorCode;
	}

	for (const string& line : lines)
	{
		vector<string> sectionRangePairs;
		FileHelper::SplitLine(line, ",", sectionRangePairs);

		vector<string> firstSectionRangeNumbers;
		FileHelper::SplitLine(sectionRangePairs[0], "-", firstSectionRangeNumbers);
		SectionRange firstRange = SectionRange(stoi(firstSectionRangeNumbers[0]), stoi(firstSectionRangeNumbers[1]));


		vector<string> secondSectionRangeNumbers;
		FileHelper::SplitLine(sectionRangePairs[1], "-", secondSectionRangeNumbers);
		SectionRange secondRange = SectionRange(stoi(secondSectionRangeNumbers[0]), stoi(secondSectionRangeNumbers[1]));

		m_SectionRangePairs.push_back(SectionRangePair(firstRange, secondRange));
	}

	return EErrorCode::Success;
}

EErrorCode CChallenge_04::Run_SecondPart()
{
	int numberOfPairsOverlapping = 0;
	for (const SectionRangePair& sectionRangePair : m_SectionRangePairs)
	{
		if (sectionRangePair.first.Overlaps(sectionRangePair.second))
			++numberOfPairsOverlapping;
	}

	cout << "There are " << numberOfPairsOverlapping << " pairs where the ranges overlap with each other." << endl;

	return EErrorCode::Success;
}

EErrorCode CChallenge_04::CleanUp_SecondPart()
{
    return EErrorCode::Success;
}
