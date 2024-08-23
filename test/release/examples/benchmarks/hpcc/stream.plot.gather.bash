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
chpl --fast stream-ep.chpl

# -----------------------------------------------------------------------------
# Run Chapel trials
# -----------------------------------------------------------------------------
for x in "${locales[@]}"; do
  size=$(echo "($CHPLEXP_SIZE_GB_PER_NODE*1024*1024*1024/8)/1" | bc)
  runAndLog ./stream-ep -nl ${x}x${CHPLEXP_NUM_SUBLOCALES} --m $size
done

# -----------------------------------------------------------------------------
# Collect data; store in results.dat
# -----------------------------------------------------------------------------
data=$(cat $runLog | sed -r -n 's/\s*max =\s*//p')
# data is reported in GB/s per node convert to total GB/s
i=0; for x in ${data[@]}; do
  nl=${locales[i]}
  scaled_data[i++]=$(echo "scale=2; $x*$nl*$CHPLEXP_NUM_SUBLOCALES" | bc -l)
done

set +x
echo -e "\t$experimentName" > $datFile
paste \
  <(printf "%s\n" "${locales[@]}") \
  <(printf "%s\n" "${scaled_data[@]}") >> "$datFile"
