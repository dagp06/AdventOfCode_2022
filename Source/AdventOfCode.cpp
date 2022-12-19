#include "Generated/GeneratedDefinitions.h"
#include <chrono>

#define TIME_NOW(varName) std::chrono::high_resolution_clock::time_point const varName = std::chrono::high_resolution_clock::now()
#define TIME_DIFF_MICRO(beginVarName, endVarName) (std::chrono::duration_cast<std::chrono::microseconds>(endVarName - beginVarName).count() / 1000000.f)


// EDIT HERE:  These lines will change what is executed
#define CHALLENGE_TYPE CChallenge_12
#define IS_FIRST_PART false

// EDIT_HERE:  Set this to true if your console closes before you have time to see your results at the end of the program
#define HOLD_EXECUTION true



int main()
{
    CHALLENGE_TYPE challenge;

    int programExecutionCount = 1;
#ifdef BENCHMARK
    programExecutionCount = 500;
#endif

    float setUpDuration = 0.f;
    float executionDuration = 0.f;
    float cleanUpDuration = 0.f;
    EErrorCode errorCode = EErrorCode::Success;

#ifdef BENCHMARK
    std::cout << "Benchmarking..." << std::endl;
#endif
    for (int i = 1; i <= programExecutionCount && errorCode == EErrorCode::Success; ++i)
    {
#ifdef BENCHMARK
        std::cout << "Iteration #" << i << std::endl;
#endif
        // Set up challenge program
        TIME_NOW(setUpBegin);
        errorCode = challenge.SetUp(IS_FIRST_PART);
        TIME_NOW(setUpEnd);
        setUpDuration += (float)TIME_DIFF_MICRO(setUpBegin, setUpEnd);

        // Execute challenge program
        TIME_NOW(executionBegin);
        if (errorCode == EErrorCode::Success)
        {
            errorCode = challenge.Run();
        }
        TIME_NOW(executionEnd);
        executionDuration += (float)TIME_DIFF_MICRO(executionBegin, executionEnd);

        // Clean up challange program
        TIME_NOW(cleanUpBegin);
        challenge.CleanUp();
        TIME_NOW(cleanUpEnd);
        cleanUpDuration += (float)TIME_DIFF_MICRO(cleanUpBegin, cleanUpEnd);
    }
    // Print timings
    std::cout << std::endl << "<========>  DONE!  <========>" << std::endl;
    std::cout << "  Program ran " << programExecutionCount << " time" << (programExecutionCount > 1 ? "s" : "") << "." << std::endl;
    std::cout << "  Set Up    :  " << setUpDuration / programExecutionCount << " s" << std::endl;
    std::cout << "  Execution :  " << executionDuration / programExecutionCount << " s" << std::endl;
    std::cout << "  Clean Up  :  " << cleanUpDuration / programExecutionCount << " s" << std::endl;

    std::cout << "  Total     :  " << (setUpDuration + executionDuration + cleanUpDuration) / programExecutionCount << " s" << std::endl;
    std::cout << "<===========================>" << std::endl;

    // Hold
    if (HOLD_EXECUTION)
    {
        int hold;
        std::cin >> hold;
    }

    return static_cast<int>(errorCode);
}
