#!/bin/bash
echo "  ==  GENERATING PROJECT SOLUTION  ==  "

PROJECT_FILE="AdventOfCode.vcxproj"
FILTERS_FILE="AdventOfCode.vcxproj.filters"

BEGIN_SYMBOL="<ItemGroup>"
END_SYMBOL="</ItemGroup>"
PATH_SEPARATOR=":::"

FOUND_DIRECTORIES=$(find "Source" -type d | tac)
FOUND_CPP_FILES=$(find "Source" -type f -name "*.cpp" | tac)
FOUND_H_FILES=$(find "Source" -type f -name "*.h" | tac)



#========================================================================================
#  Directory filters
#========================================================================================
echo "  >>  Generating source directories filters..."
FILTERS_BEGIN=$(grep -n $BEGIN_SYMBOL $FILTERS_FILE | cut -d: -f1 | head -n1)
FILTERS_END=$(grep -n $END_SYMBOL $FILTERS_FILE | cut -d: -f1 | head -n1)

# Delete the previous directory filter entries
if [ $(($FILTERS_END - $FILTERS_BEGIN - 1)) -gt 0 ]; then
    START=$(($FILTERS_BEGIN + 1))
    STOP=$(($FILTERS_END - 1))
    sed -i "$START,$STOP d" $FILTERS_FILE
    echo "      -  [$START, $STOP]  @  $FILTERS_FILE"
fi

# Add the current directory filter entries
echo "$FOUND_DIRECTORIES" | while read -r DIRECTORY; do
    ADJUSTED_DIRECTORY=$(echo $DIRECTORY | sed "s?/?$PATH_SEPARATOR?g")
    sed -i "$FILTERS_BEGIN a\\    <Filter Include=\"$ADJUSTED_DIRECTORY\"/>" $FILTERS_FILE
    echo "      +  $DIRECTORY"
done



#========================================================================================
#  Rebuild .cpp file entries
#========================================================================================
echo "  >>  Generating .cpp files inclusion"

PROJECT_BEGIN=$(grep -n $BEGIN_SYMBOL $PROJECT_FILE | cut -d: -f1 | tail -n2 | head -n1)
PROJECT_END=$(grep -n $END_SYMBOL $PROJECT_FILE | cut -d: -f1 | tail -n2 | head -n1)
FILTERS_BEGIN=$(grep -n $BEGIN_SYMBOL $FILTERS_FILE | cut -d: -f1 | head -n2 | tail -n1)
FILTERS_END=$(grep -n $END_SYMBOL $FILTERS_FILE | cut -d: -f1 | head -n2 | tail -n1)

# Delete the previous .cpp entries from the project file
if [ $(($PROJECT_END - $PROJECT_BEGIN - 1)) -gt 0 ]; then
    START=$(($PROJECT_BEGIN + 1))
    STOP=$(($PROJECT_END - 1))
    sed -i "$START,$STOP d" $PROJECT_FILE
    echo "      -  [$START, $STOP]  @  $PROJECT_FILE"
fi

# Delete the previous .cpp entries from the filters file
if [ $(($FILTERS_END - $FILTERS_BEGIN - 1)) -gt 0 ]; then
    START=$(($FILTERS_BEGIN + 1))
    STOP=$(($FILTERS_END - 1))
    sed -i "$START,$STOP d" $FILTERS_FILE
    echo "      -  [$START, $STOP]  @  $FILTERS_FILE"
fi

# Add the current .cpp entries
echo "$FOUND_CPP_FILES" | while read -r FILE; do
    ADJUSTED_FILE=$(echo $FILE | sed "s?/?$PATH_SEPARATOR?g")
    FILE_DIRECTORY="${ADJUSTED_FILE%$PATH_SEPARATOR*}"

    sed -i "$PROJECT_BEGIN a\\    <ClCompile Include=\"$ADJUSTED_FILE\"/>" $PROJECT_FILE

    sed -i "$FILTERS_BEGIN a\\    </ClCompile>" $FILTERS_FILE
    sed -i "$FILTERS_BEGIN a\\      <Filter>$FILE_DIRECTORY</Filter>" $FILTERS_FILE
    sed -i "$FILTERS_BEGIN a\\    <ClCompile Include=\"$ADJUSTED_FILE\">" $FILTERS_FILE

    echo "      +  $FILE"
done



#========================================================================================
#  Rebuild .h file entries
#========================================================================================
echo "  >>  Generating .h files inclusion"

PROJECT_BEGIN=$(grep -n $BEGIN_SYMBOL $PROJECT_FILE | cut -d: -f1 | tail -n1)
PROJECT_END=$(grep -n $END_SYMBOL $PROJECT_FILE | cut -d: -f1 | tail -n1)
FILTERS_BEGIN=$(grep -n $BEGIN_SYMBOL $FILTERS_FILE | cut -d: -f1 | head -n3 | tail -n1)
FILTERS_END=$(grep -n $END_SYMBOL $FILTERS_FILE | cut -d: -f1 | head -n3 | tail -n1)

# Delete the previous .h entries from the project file
if [ $(($PROJECT_END - $PROJECT_BEGIN - 1)) -gt 0 ]; then
    START=$(($PROJECT_BEGIN + 1))
    STOP=$(($PROJECT_END - 1))
    sed -i "$START,$STOP d" $PROJECT_FILE
    echo "      -  [$START, $STOP]  @  $PROJECT_FILE"
fi

# Delete the previous .h entries from the filters file
if [ $(($FILTERS_END - $FILTERS_BEGIN - 1)) -gt 0 ]; then
    START=$(($FILTERS_BEGIN + 1))
    STOP=$(($FILTERS_END - 1))
    sed -i "$START,$STOP d" $FILTERS_FILE
    echo "      -  [$START, $STOP]  @  $FILTERS_FILE"
fi

# Add the current .h entries
echo "$FOUND_H_FILES" | while read -r FILE; do
    ADJUSTED_FILE=$(echo $FILE | sed "s?/?$PATH_SEPARATOR?g")
    FILE_DIRECTORY="${ADJUSTED_FILE%$PATH_SEPARATOR*}"

    sed -i "$PROJECT_BEGIN a\\    <ClInclude Include=\"$ADJUSTED_FILE\"/>" $PROJECT_FILE

    sed -i "$FILTERS_BEGIN a\\    </ClInclude>" $FILTERS_FILE
    sed -i "$FILTERS_BEGIN a\\      <Filter>$FILE_DIRECTORY</Filter>" $FILTERS_FILE
    sed -i "$FILTERS_BEGIN a\\    <ClInclude Include=\"$ADJUSTED_FILE\">" $FILTERS_FILE

    echo "      +  $FILE"
done


#========================================================================================
#  Replace all the separators
#========================================================================================
echo "  >>  Replacing temporary path separators..."
sed -i "s?$PATH_SEPARATOR?\\\\?g" $PROJECT_FILE
sed -i "s?$PATH_SEPARATOR?\\\\?g" $FILTERS_FILE



echo "  >>  ...DONE!"
