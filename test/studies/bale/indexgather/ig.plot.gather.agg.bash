#!/usr/bin/env bash

# sets 'datFile', 'logDir', 'experimentName', and 'runLog'
source $CHPL_HOME/util/test/chplExperimentGatherUtils/prelude.bash $@

locales=( 2 4 8 16 32 64 128 256 512 1024)
capLocales "$CHPLEXP_MAX_LOCALES"
nodesPerLocale=2

# -----------------------------------------------------------------------------
# Build Chapel code
# -----------------------------------------------------------------------------
pushd ../aggregation
chpl --fast ig.chpl -sN=10000000 -suseBlockArr=true

# -----------------------------------------------------------------------------
# Run Chapel trials
# -----------------------------------------------------------------------------
for x in "${locales[@]}"; do
  launchAndLog "${x}x${CHPLEXP_NUM_SUBLOCALES}" ./ig
done
popd

# -----------------------------------------------------------------------------
# Collect data; store in results.dat
# -----------------------------------------------------------------------------
data=$(cat $runLog | sed -r -n 's/\s+AGG:.*seconds\s+//p' | cut -d' ' -f 1)
set +x

# data is reported in GB/s per node convert to total GB/s
i=0; for x in ${data[@]}; do
  nl=${locales[i]}
  scaled_data[i++]=$(echo "$x*$CHPLEXP_NUM_SUBLOCALES*$nl" | bc -l)
done
echo -e "\t$experimentName" > $datFile
paste \
  <(printf "%s\n" "${locales[@]}") \
  <(printf "%s\n" "${scaled_data[@]}") >> "$datFile"
