#pragma once
#include "../CommonDefinitions.h"

namespace FileHelper
{
    EErrorCode ReadFirstLine(std::string const& filePath, std::string& outLine);
    EErrorCode ReadLines(std::string const& filePath, std::vector<std::string>& outLines);
    void SplitLine(std::string const& line, std::string const& delimiter, std::vector<std::string>& outItems);
};
