# Source this script at the top of your gather scripts to automatically:
#
# - set datFile, logDir, experimentName, runLog
# - print these values out
# - start logging

# -- Command line arguments --
datFile="${1:-`pwd`/logs/experiment.dat}"
logDir="${2:-`pwd`/logs}"
experimentName="${3:-experiment}"

# -- Testing parameters --
runLog="$logDir/$experimentName.txt"

#  Debugging output
echoVar() { echo "$1 = ${!1}"; }
echoVar datFile
echoVar logDir
echoVar experimentName
echoVar runLog
echo "------------------"
mkdir -p "$logDir"
set -e -x
