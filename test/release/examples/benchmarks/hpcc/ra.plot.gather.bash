#!/usr/bin/env bash

# sets 'datFile', 'logDir', 'experimentName', and 'runLog'
source $CHPL_HOME/util/test/chplExperimentGatherUtils/prelude.bash $@

locales=( 2 4 8 16 32 64 128 256 512 1024)
capLocales "$CHPLEXP_MAX_LOCALES"

# -----------------------------------------------------------------------------
# Build Chapel code
# -----------------------------------------------------------------------------
chpl --fast ra.chpl -suseOn=false -sn=33:indexType -sN_U=1000000000:indexType -sverify=false

# -----------------------------------------------------------------------------
# Run Chapel trials
# -----------------------------------------------------------------------------
for x in "${locales[@]}"; do
  runAndLog ./ra -nl $x --n=16 --N_U=4096
done

# -----------------------------------------------------------------------------
# Collect data; store in results.dat
# -----------------------------------------------------------------------------
data=$(cat $runLog | sed -r -n 's/Performance \(GUPS\) = //p')
set +x

echo -e "\t$experimentName" > $datFile
paste \
  <(printf "%s\n" "${locales[@]}") \
  <(printf "%s\n" "${data[@]}") >> "$datFile"
