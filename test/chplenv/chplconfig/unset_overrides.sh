# Unset any env vars currently set
CHPL_OVERRIDES=$(printchplenv --overrides | awk -F'=' '{print $1}')
for CHPL_VAR in ${CHPL_OVERRIDES}; do
    if [ ${CHPL_VAR} != "CHPL_HOME" ]; then
        unset ${CHPL_VAR}
    fi
done

