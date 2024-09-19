#!/usr/bin/env bash

# sets 'datFile', 'logDir', 'experimentName', and 'runLog'
source $CHPL_HOME/util/test/chplExperimentGatherUtils/prelude.bash $@

locales=( 2 4 8 16 32 64 128 256 512 1024)
nodesPerLocale=2
capLocales "$CHPLEXP_MAX_LOCALES"

# -----------------------------------------------------------------------------
# Build Chapel code
# -----------------------------------------------------------------------------
chpl --fast -M ../aggregation ig.chpl

# -----------------------------------------------------------------------------
# Run Chapel trials
# -----------------------------------------------------------------------------
PROBLEM_SIZE=1000000
for x in "${locales[@]}"; do
  launchAndLog "${x}x${CHPLEXP_NUM_SUBLOCALES}" ./ig
done

# -----------------------------------------------------------------------------
# Collect data; store in results.dat
# -----------------------------------------------------------------------------
data=$(cat $runLog | sed -r -n 's/MB\/s per node: //p')
set +x

# data is reported in MB/s per node convert to total GB/s
i=0; for x in ${data[@]}; do
  nl=${locales[i]}
  scaled_data[i++]=$(echo "$x*$CHPLEXP_NUM_SUBLOCALES*$nl/1000" | bc -l)
done
echo -e "\t$experimentName" > $datFile
paste \
  <(printf "%s\n" "${locales[@]}") \
  <(printf "%s\n" "${scaled_data[@]}") >> "$datFile"
