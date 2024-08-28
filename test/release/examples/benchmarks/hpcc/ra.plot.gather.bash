#!/usr/bin/env bash

# sets 'datFile', 'logDir', 'experimentName', and 'runLog'
source $CHPL_HOME/util/test/chplExperimentGatherUtils/prelude.bash $@

CHPLEXP_USE_ON="${CHPLEXP_USE_ON:false}"
CHPLEXP_USE_ATOMICS="${CHPLEXP_USE_ATOMICS:false}"

locales=( 2 4 8 16 32 64 128 256 512 1024)
capLocales "$CHPLEXP_MAX_LOCALES"

# -----------------------------------------------------------------------------
# Build Chapel code
# -----------------------------------------------------------------------------
if [[ "$CHPLEXP_USE_ATOMICS" == "true" ]]; then
  chpl --fast ra-atomics.chpl
  exe_name="./ra-atomics"
else
  chpl --fast ra.chpl -suseOn=$CHPLEXP_USE_ON
  exe_name="./ra"
fi

# -----------------------------------------------------------------------------
# Run Chapel trials
# -----------------------------------------------------------------------------
ra_lg_size=32
ra_updates=2097152
for x in "${locales[@]}"; do
  launchAndLog $x $exe_name --n=$ra_lg_size --N_U=$ra_updates
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
