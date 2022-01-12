#!/bin/bash

if [[ -z ${FABTEST_PATH} ]] ; then
	export FABTEST_PATH=/scratch/jenkins/builds/fabtests/stable
fi

export PATH=${FABTEST_PATH}/bin:$PATH

if [[ -z ${FABTEST_PROVIDER} ]] ; then
    FABTEST_PROVIDER='verbs;ofi_rxm'
fi

if [[ -z ${SLURMD_NODENAME} ]] ; then
    echo SLURM is the only supported WLM at this point for testing
    exit 1
fi

SERVER=$(scontrol show hostname ${SLURM_NODELIST} | head -n1)
CLIENT=$(scontrol show hostname ${SLURM_NODELIST} | tail -n1)

SERVER_ADDR=$(getent hosts ${SERVER} | awk '{print $1}')
CLIENT_ADDR=$(getent hosts ${CLIENT} | awk '{print $1}')

if [[ "${SLURMD_NODENAME}" == "$SERVER" ]] ; then
    runfabtests.sh \
        $@ \
        "${FABTEST_PROVIDER}" \
        ${SERVER_ADDR} \
        ${CLIENT_ADDR}
fi
