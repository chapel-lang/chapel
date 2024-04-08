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
echo -e "\t$experimentName" > $datFile
paste \
  <(printf "%s\n" "${locales[@]}") \
  <(printf "%s\n" "${data[@]}") >> "$datFile"
