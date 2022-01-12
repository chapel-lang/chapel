#!/usr/bin/env bats

load test_helper

@test "fi_info" {
    run ${CONTRIB_BIN}/logwrap -w ${BATS_TEST_LOGFILE} -- $(batch_launcher 1 1) ${LIBFABRIC_INSTALL_PATH}/bin/fi_info -p 'verbs;ofi_rxm'
    [ $(grep 'provider: verbs;ofi_rxm' ${BATS_TEST_LOGFILE} | uniq | wc -l) -gt 0 ]
    [ "$status" -eq 0 ]
}

@test "fi_info_xrc" {
    FI_OFI_RXM_USE_SRX=1 run ${CONTRIB_BIN}/logwrap -w ${BATS_TEST_LOGFILE} -- $(batch_launcher 1 1) ${LIBFABRIC_INSTALL_PATH}/bin/fi_info -p 'verbs;ofi_rxm'
    [ $(grep 'domain' ${BATS_TEST_LOGFILE} | grep '\-xrc' | wc -l) -gt 0 ]
    [ "$status" -eq 0 ]
}

@test "prefer_rc" {
    FI_OFI_RXM_USE_SRX=1 run ${CONTRIB_BIN}/logwrap -w ${BATS_TEST_LOGFILE} -- $(batch_launcher 1 1) ${LIBFABRIC_INSTALL_PATH}/bin/fi_info -p 'verbs;ofi_rxm'
    [ $(grep 'domain' ${BATS_TEST_LOGFILE} | head -n1 | grep '\-xrc' | wc -l) -eq 0 ]
    [ "$status" -eq 0 ]
}

@test "prefer_xrc" {
    FI_VERBS_PREFER_XRC=1 FI_OFI_RXM_USE_SRX=1 run ${CONTRIB_BIN}/logwrap -w ${BATS_TEST_LOGFILE} -- $(batch_launcher 1 1) ${LIBFABRIC_INSTALL_PATH}/bin/fi_info -p 'verbs;ofi_rxm'
    [ $(grep 'domain' ${BATS_TEST_LOGFILE} | head -n1 | grep '\-xrc' | wc -l) -gt 0 ]
    [ "$status" -eq 0 ]
}
