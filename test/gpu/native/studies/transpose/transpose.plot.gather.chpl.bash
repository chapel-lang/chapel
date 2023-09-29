#!/usr/bin/env bash

# sets 'datFile', 'logDir', 'experimentName', and 'runLog'
source $CHPL_HOME/util/test/chplExperimentGatherUtils/prelude.bash $@

sizes=( 2 4 6 8 10 12 14 16)

# -----------------------------------------------------------------------------
# Build Chapel code
# -----------------------------------------------------------------------------
chpl transpose.chpl

# -----------------------------------------------------------------------------
# Run Chapel trials
# -----------------------------------------------------------------------------
for x in "${sizes[@]}"; do
  x=$((x * 1000))
  runAndSubLog naive ./transpose --perftest=true --sizeX=$x --sizeY=$x --impl naive
  runAndSubLog clever ./transpose --perftest=true --sizeX=$x --sizeY=$x --impl clever
  runAndSubLog lowlevel ./transpose --perftest=true --sizeX=$x --sizeY=$x --impl lowlevel
done

# -----------------------------------------------------------------------------
# Collect data; store in results.dat
# -----------------------------------------------------------------------------
naive_data=$(cat $runLog.naive | sed -r -n 's/Performance \(GB\/s\): //p' | tr -s ' ' | cut -d ' ' -f 3)
clever_data=$(cat $runLog.clever | sed -r -n 's/Performance \(GB\/s\): //p' | tr -s ' ' | cut -d ' ' -f 3)
lowlevel_data=$(cat $runLog.lowlevel | sed -r -n 's/Performance \(GB\/s\): //p' | tr -s ' ' | cut -d ' ' -f 3)

set +x
echo -e "\t$experimentName.naive\t$experimentName.clever\t$experimentName.lowlevel" > $datFile
paste \
  <(printf "%s\n" "${sizes[@]}") \
  <(printf "%s\n" "${naive_data[@]}") \
  <(printf "%s\n" "${clever_data[@]}") \
  <(printf "%s\n" "${lowlevel_data[@]}") >> "$datFile"
