# Source this script at the top of your gather scripts to automatically:
#
# - set datFile, logDir, experimentName, runLog
# - print these values out
# - start logging

# -- Command line arguments --
scriptInvokeDir=`pwd`
datFile="${1:-`pwd`/logs/experiment.dat}"
logDir="${2:-`pwd`/logs}"
experimentName="${3:-experiment}"

function runAndLog () {
  exec $@ | tee -a $runLog
}

function runAndSubLog () {
  sublog=$1
  shift
  exec $@ | tee -a $runLog.$sublog
}

function extractFromPerfKeys () {
  $CHPL_HOME/util/test/chplExperimentGatherUtils/gatherFromPerfKeys.py --fromDir "$scriptInvokeDir" --runlog $runLog $@
}

# -- Testing parameters --
runLog="$logDir/$experimentName.txt"
echo "" > $runLog

#  Debugging output
echoVar() { echo "$1 = ${!1}"; }
echoVar datFile
echoVar logDir
echoVar experimentName
echoVar runLog
echo "------------------"
mkdir -p "$logDir"
set -e -x
