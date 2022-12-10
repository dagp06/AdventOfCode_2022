#!/bin/bash
echo "  ==  DOWNLOADING INPUT FILE  ==  "
YEAR=${1}
DAY=${2}
SESSION_KEY=${3}

DAY_NO_ZERO=$(echo $DAY | sed "s/^0*//")
OUTPUT_FILE="Input_Challenge_$DAY.txt"

echo "  >>  Downloading file from 'https://adventofcode.com/'..."
URL="https://adventofcode.com/$YEAR/day/$DAY_NO_ZERO/input"
curl $URL -H "cookie: session=$SESSION_KEY" -o $OUTPUT_FILE

echo "  >>  ...DONE!"
