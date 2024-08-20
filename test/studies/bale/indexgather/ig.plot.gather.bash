#!/usr/bin/env bash

# sets 'datFile', 'logDir', 'experimentName', and 'runLog'
source $CHPL_HOME/util/test/chplExperimentGatherUtils/prelude.bash $@

CHPLEXP_MAX_LOCALES=32
locales=( 2 4 8 16 32 64 128 256 512 1024)
if [ ! -z "$CHPLEXP_MAX_LOCALES" ]; then
  filtered_locales=()
  for value in "${locales[@]}"; do
    ((value <= $CHPLEXP_MAX_LOCALES)) && filtered_locales+=("$value")
  done
  locales=("${filtered_locales[@]}")
fi
echo "LOCALES TO CHECK: ${locales[@]}"

# -----------------------------------------------------------------------------
# Build Chapel code
# -----------------------------------------------------------------------------
chpl --fast --no-optimize-forall-unordered-ops -M ../aggregation ig.chpl

# -----------------------------------------------------------------------------
# Run Chapel trials
# -----------------------------------------------------------------------------
PROBLEM_SIZE=1000000
for x in "${locales[@]}"; do
  runAndLog ./ig -nl $x --N=$PROBLEM_SIZE --printStats --mode=Mode.ordered
done

# -----------------------------------------------------------------------------
# Collect data; store in results.dat
# -----------------------------------------------------------------------------
data=$(cat $runLog | sed -r -n 's/MB\/s per node: //p')
set +x

# data is reported in MB/s per mode convert to GB/s
i=0; for x in ${data[@]}; do
  nl=${locales[i]}
  scaled_data[i++]=$(echo "$x*$nl/1000" | bc -l)
done
echo -e "\t$experimentName" > $datFile
paste \
  <(printf "%s\n" "${locales[@]}") \
  <(printf "%s\n" "${scaled_data[@]}") >> "$datFile"
