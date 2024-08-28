#!/usr/bin/env bash

# sets 'datFile', 'logDir', 'experimentName', and 'runLog'
source $CHPL_HOME/util/test/chplExperimentGatherUtils/prelude.bash $@

CHPLEXP_NUM_SUBLOCALES="${CHPLEXP_NUM_SUBLOCALES:-1}"
CHPLEXP_SIZE_GB_PER_NODE="${CHPLEXP_SIZE_GB_PER_NODE:-32}"

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
  size=$(echo "($CHPLEXP_SIZE_GB_PER_NODE*1024*1024*1024/8/$CHPLEXP_NUM_SUBLOCALES)/1" | bc)
  launchAndLog "${x}x${CHPLEXP_NUM_SUBLOCALES}" ./stream --m $size
done

# -----------------------------------------------------------------------------
# Collect data; store in results.dat
# -----------------------------------------------------------------------------
data=$(cat $runLog | sed -r -n 's/\s*Performance \(GB\/s\) =\s*//p')
set +x
echo -e "\t$experimentName" > $datFile
paste \
  <(printf "%s\n" "${locales[@]}") \
  <(printf "%s\n" "${data[@]}") >> "$datFile"
