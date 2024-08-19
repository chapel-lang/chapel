#!/usr/bin/env bash

# sets 'datFile', 'logDir', 'experimentName', and 'runLog'
source $CHPL_HOME/util/test/chplExperimentGatherUtils/prelude.bash $@

locales=( 2 4 8 16 32 64 128 256 512 1024)
capLocales "$CHPLEXP_MAX_LOCALES"

# -----------------------------------------------------------------------------
# Build Chapel code
# -----------------------------------------------------------------------------
chpl --fast stream.chpl

# -----------------------------------------------------------------------------
# Run Chapel trials
# -----------------------------------------------------------------------------
for x in "${locales[@]}"; do
  runAndLog ./stream -nl $x --m 5723827200
done

# -----------------------------------------------------------------------------
# Collect data; store in results.dat
# -----------------------------------------------------------------------------
data=$(cat $runLog | sed -r -n 's/Performance \(GB\/s\) = //p')
set +x

# data is reported in GB/s convert to MB/s
i=0; for x in ${data[@]}; do
    scaled_data[i++]=$(echo "$x*1000" | bc -l)
done
echo -e "\t$experimentName" > $datFile
paste \
  <(printf "%s\n" "${locales[@]}") \
  <(printf "%s\n" "${scaled_data[@]}") >> "$datFile"
