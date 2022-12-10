#!/bin/bash
echo "  ==  GENERATING CODE DEFINITIONS  ==  "

DEFINITIONS_FILE="GeneratedDefinitions.h"
PATH_SEPARATOR=":::"
FOUND_H_FILES=$(find "../Challenges" -maxdepth 1 -type f -name "*.h" | tac)

#========================================================================================
#  Challenge includes
#========================================================================================
echo "  >>  Generating challenge includes region..."
REGION_BEGIN=$(grep -n "#pragma region challenge_includes" $DEFINITIONS_FILE | cut -d: -f1 | head -n1)
REGION_END=$(grep -n "#pragma endregion challenge_includes" $DEFINITIONS_FILE | cut -d: -f1 | head -n1)

# Delete the content of the region
if [ $(($REGION_END - $REGION_BEGIN - 1)) -gt 0 ]; then
    START=$(($REGION_BEGIN + 1))
    STOP=$(($REGION_END - 1))
    sed -i "$START,$STOP d" $DEFINITIONS_FILE
    echo "      -  [$START, $STOP]  @  $DEFINITIONS_FILE"
fi

# Add the current .h entries
echo "$FOUND_H_FILES" | while read -r FILE; do
    sed -i "$REGION_BEGIN a\\#include \"$FILE\"" $DEFINITIONS_FILE
    echo "      +  $FILE"
done



echo "  >>  ...DONE!"
