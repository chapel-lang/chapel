# Modify environment, removing any non-home CHPL_* vars and add $PRINTCHPLENV

export PRINTCHPLENV=$CHPL_HOME/util/printchplenv

CHPL_OVERRIDES=$(${PRINTCHPLENV} --overrides --simple | awk -F'=' '{print $1}')

for CHPL_VAR in ${CHPL_OVERRIDES}; do
    if [ ${CHPL_VAR} != "CHPL_HOME" ]; then
        unset ${CHPL_VAR}
    fi
done

