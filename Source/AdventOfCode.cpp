#include "Generated/GeneratedDefinitions.h"
#include <chrono>

#define TIME_NOW(varName) std::chrono::high_resolution_clock::time_point const varName = std::chrono::high_resolution_clock::now()
#define TIME_DIFF_MICRO(beginVarName, endVarName) (std::chrono::duration_cast<std::chrono::microseconds>(endVarName - beginVarName).count() / 1000000.f)


// EDIT HERE:  These lines will change what is executed
#define CHALLENGE_TYPE CChallenge_05
#define IS_FIRST_PART false

// EDIT_HERE:  Set this to true if your console closes before you have time to see your results at the end of the program
#define HOLD_EXECUTION true



int main()
{
    CHALLENGE_TYPE challenge;

    // Set up challenge program
    TIME_NOW(setUpBegin);
    EErrorCode errorCode = challenge.SetUp(IS_FIRST_PART);
    TIME_NOW(setUpEnd);

    // Execute challenge program
    TIME_NOW(executionBegin);
    if (errorCode == EErrorCode::Success)
    {
        errorCode = challenge.Run();
    }
    TIME_NOW(executionEnd);

    // Clean up challange program
    TIME_NOW(cleanUpBegin);
    challenge.CleanUp();
    TIME_NOW(cleanUpEnd);

    // Print timings
    std::cout << std::endl << "<========>  DONE!  <========>" << std::endl;
    std::cout << "  Set Up    :  " << TIME_DIFF_MICRO(setUpBegin, setUpEnd) << " s" << std::endl;
    std::cout << "  Execution :  " << TIME_DIFF_MICRO(executionBegin, executionEnd) << " s" << std::endl;
    std::cout << "  Clean Up  :  " << TIME_DIFF_MICRO(cleanUpBegin, cleanUpEnd) << " s" << std::endl;
    std::cout << "<===========================>" << std::endl;

    // Hold
    if (HOLD_EXECUTION)
    {
        int hold;
        std::cin >> hold;
    }

    return static_cast<int>(errorCode);
}
