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

function launchAndLog () {
  local cmd=${2}
  local args=${@:3}

  # Strip out colocales if there's only one
  nl="$1"
  if [[ $nl = *x1 ]]; then
    nl=$(echo "$nl" | sed -e "s/x1$//")
  fi

  echo "UNEVAL LAUNCHCMD=$CHPLEXP_LAUNCH_COMMAND"
  local launchCmd=$(eval "echo $CHPLEXP_LAUNCH_COMMAND")
  echo "LAUNCH COMMAND IS: $launchCmd"

  if [[ ! -z "$CHPLEXP_PRELAUNCH_COMMAND" ]]; then
    eval "$CHPLEXP_PRELAUNCH_COMMAND"
  fi

  runAndLog "$launchCmd"

  if [[ ! -z "$CHPLEXP_POSTLAUNCH_COMMAND" ]]; then
    eval "$CHPLEXP_POSTLAUNCH_COMMAND"
  fi
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
