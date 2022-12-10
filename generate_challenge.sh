#!/bin/bash
YEAR="${1}"
DAY="${2}"

if [[ (-z $YEAR) || (-z $DAY) ]]; then
    echo "ERROR!  Invalid arguments."
    echo "        Command  ->  sh generate_challenge.sh [YEAR] [DAY]"
    echo "        Example  ->  sh generate_challenge.sh 2020 01"
    exit 1
fi

SESSION_KEY_FILE="session_key.txt"
SESSION_KEY=$(<$SESSION_KEY_FILE)

if [[ -z "$SESSION_KEY" ]]; then
    echo "ERROR!  No session key specified."
    echo "        To find a session key, inspect the input web page's cookies."
    echo "        Add the key to '$SESSION_KEY_FILE' and run this script again."
    exit 1
fi

cd "Inputs/"
sh "download_input.sh" $YEAR $DAY $SESSION_KEY
echo ""

cd "../Source/Challenges/"
sh "generate_challenge_code.sh" $DAY
echo ""

cd "../Generated/"
sh "generate_definitions_code.sh"
echo ""

cd "../../"
sh "generate_solution.sh"