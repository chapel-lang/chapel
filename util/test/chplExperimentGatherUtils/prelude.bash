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

capLocales() {
  local max="$1"

  if [ ! -z "$max" ]; then
    filtered_arr=()
    for value in "${locales[@]}"; do
      ((value <= $max)) && filtered_arr+=("$value")
    done
    locales=("${filtered_arr[@]}")
  fi
}

function runAndLog () {
  exec $@ 2>&1 | tee -a $runLog
}

function runAndSubLog () {
  sublog=$1
  shift
  exec $@ 2>&1 | tee -a $runLog.$sublog
}

function runAndLog_launch () {
  local constraintsSanitized=$(echo "$CHPL_LAUNCHER_CONSTRAINT" | sed -n 's/,/&/g')
  runAndLog srun --nodes $1 --constraint="$CHPL_LAUNCHER_CONSTRAINT"  "${@:2}"
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
