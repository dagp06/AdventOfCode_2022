#include "FileHelpers.h"
#include <fstream>



EErrorCode FileHelper::ReadFirstLine(std::string const& filePath, std::string& outLine)
{
    std::ifstream fileToRead(filePath);
    if (!fileToRead.is_open())
    {
        return EErrorCode::FileNotFound;
    }

    std::getline(fileToRead, outLine);
    fileToRead.close();

    return outLine.empty() ? EErrorCode::FileEmpty : EErrorCode::Success;
}

EErrorCode FileHelper::ReadLines(std::string const& filePath, std::vector<std::string>& outLines)
{
    std::ifstream fileToRead(filePath);
    if (!fileToRead.is_open())
    {
        return EErrorCode::FileNotFound;
    }

    bool isFileEmpty = true;
    std::string line;

    while (std::getline(fileToRead, line))
    {
        outLines.push_back(line);
        isFileEmpty = isFileEmpty && line.empty();
    }
    fileToRead.close();

    return isFileEmpty ? EErrorCode::FileEmpty : EErrorCode::Success;
}

void FileHelper::SplitLine(std::string const& line, std::string const& delimiter, std::vector<std::string>& outItems)
{
    size_t beginIndex = 0;
    size_t endIndex = 0;

    size_t const lineSize = line.size();
    size_t const delimiterSize = delimiter.size();

    while (beginIndex < lineSize)
    {
        size_t const delimiterIndex = line.find(delimiter, beginIndex);
        endIndex = delimiterIndex == std::string::npos ? lineSize : delimiterIndex;

        outItems.push_back(line.substr(beginIndex, endIndex - beginIndex));
        beginIndex = endIndex + delimiterSize;
    }
}
