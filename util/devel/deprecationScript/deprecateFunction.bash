#!/bin/bash

# Run first script
echo extractLineNumbers.py "${1}" "${2}"
python3 $CHPL_HOME/util/devel/deprecationScript/extractLineNumbers.py "${1}" "${2}" warning_lines.txt

# Check if the extractLineNumbers succeeded and produced the file with line numbers
if [ -f warning_lines.txt ]; then
    # Run second python script using the intermediate file as input
    echo replaceDeprecatedLines.py warning_lines.txt "${3}" "${4}"
    python3 $CHPL_HOME/util/devel/deprecationScript/replaceDeprecatedLines.py "${3}" "${4}" warning_lines.txt

    # Cleanup the intermediate file
    echo "Removing warning_lines.txt"
    rm warning_lines.txt
else
    echo "warning_lines.txt file not found. Skipping second script."
fi
