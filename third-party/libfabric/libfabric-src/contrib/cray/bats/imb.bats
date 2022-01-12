#!/usr/bin/env bats

load test_helper

XRC_FI_ENV="FI_VERBS_XRCD_FILENAME=/tmp/xrc_imb_$$ FI_OFI_RXM_USE_SRX=1 FI_VERBS_PREFER_XRC=1"

# RC
@test "IMB-P2P unirandom 2 ranks, 1 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 2 1) timeout 300 "$IMB_BUILD_PATH/IMB-P2P -npmin 2 -time 10 -mem 2 -msglog 2:18 unirandom"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-P2P unirandom 2 ranks, 1 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 2 1) timeout 300 "$IMB_BUILD_PATH/IMB-P2P -npmin 2 -time 10 -mem 2 -msglog 2:18 unirandom"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-P2P birandom 2 ranks, 1 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 2 1) timeout 300 "$IMB_BUILD_PATH/IMB-P2P -npmin 2 -time 10 -mem 2 -msglog 2:18 birandom"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-P2P birandom 2 ranks, 1 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 2 1) timeout 300 "$IMB_BUILD_PATH/IMB-P2P -npmin 2 -time 10 -mem 2 -msglog 2:18 birandom"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-P2P corandom 2 ranks, 1 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 2 1) timeout 300 "$IMB_BUILD_PATH/IMB-P2P -npmin 2 -time 10 -mem 2 -msglog 2:18 corandom"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-P2P corandom 2 ranks, 1 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 2 1) timeout 300 "$IMB_BUILD_PATH/IMB-P2P -npmin 2 -time 10 -mem 2 -msglog 2:18 corandom"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-RMA bidir_get 2 ranks, 1 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 2 1) timeout 300 "$IMB_BUILD_PATH/IMB-RMA -npmin 2 -time 10 -mem 2 -msglog 2:18 bidir_get"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-RMA bidir_get 2 ranks, 1 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 2 1) timeout 300 "$IMB_BUILD_PATH/IMB-RMA -npmin 2 -time 10 -mem 2 -msglog 2:18 bidir_get"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-RMA bidir_put 2 ranks, 1 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 2 1) timeout 300 "$IMB_BUILD_PATH/IMB-RMA -npmin 2 -time 10 -mem 2 -msglog 2:18 bidir_put"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-RMA bidir_put 2 ranks, 1 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 2 1) timeout 300 "$IMB_BUILD_PATH/IMB-RMA -npmin 2 -time 10 -mem 2 -msglog 2:18 bidir_put"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-RMA unidir_get 2 ranks, 1 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 2 1) timeout 300 "$IMB_BUILD_PATH/IMB-RMA -npmin 2 -time 10 -mem 2 -msglog 2:18 unidir_get"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-RMA unidir_get 2 ranks, 1 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 2 1) timeout 300 "$IMB_BUILD_PATH/IMB-RMA -npmin 2 -time 10 -mem 2 -msglog 2:18 unidir_get"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-RMA unidir_put 2 ranks, 1 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 2 1) timeout 300 "$IMB_BUILD_PATH/IMB-RMA -npmin 2 -time 10 -mem 2 -msglog 2:18 unidir_put"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-RMA unidir_put 2 ranks, 1 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 2 1) timeout 300 "$IMB_BUILD_PATH/IMB-RMA -npmin 2 -time 10 -mem 2 -msglog 2:18 unidir_put"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-EXT window 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-EXT -npmin 20 -time 10 -mem 2 -msglog 2:18 window"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-EXT window 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-EXT -npmin 20 -time 10 -mem 2 -msglog 2:18 window"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-EXT accumulate 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-EXT -npmin 20 -time 10 -mem 2 -msglog 2:18 accumulate"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-EXT accumulate 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-EXT -npmin 20 -time 10 -mem 2 -msglog 2:18 accumulate"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-NBC ialltoall 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 ialltoall"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-NBC ialltoall 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 ialltoall"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-NBC ialltoall_pure 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 ialltoall_pure"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-NBC ialltoall_pure 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 ialltoall_pure"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-NBC ialltoallv 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 ialltoallv"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-NBC ialltoallv 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 ialltoallv"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-NBC ialltoallv_pure 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 ialltoallv_pure"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-NBC ialltoallv_pure 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 ialltoallv_pure"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-NBC iallgather 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 iallgather"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-NBC iallgather 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 iallgather"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-NBC iallgather_pure 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 iallgather_pure"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-NBC iallgather_pure 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 iallgather_pure"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-NBC iallgatherv 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 iallgatherv"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-NBC iallgatherv 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 iallgatherv"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-NBC iallgatherv_pure 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 iallgatherv_pure"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-NBC iallgatherv_pure 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 iallgatherv_pure"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-NBC iallreduce 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 iallreduce"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-NBC iallreduce 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 iallreduce"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-NBC iallreduce_pure 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 iallreduce_pure"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-NBC iallreduce_pure 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 iallreduce_pure"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-NBC ibarrier 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 ibarrier"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-NBC ibarrier 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 ibarrier"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-NBC ibarrier_pure 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 ibarrier_pure"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-NBC ibarrier_pure 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 ibarrier_pure"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-NBC ibcast 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 ibcast"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-NBC ibcast 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 ibcast"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-NBC ibcast_pure 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 ibcast_pure"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-NBC ibcast_pure 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 ibcast_pure"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-NBC igather 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 igather"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-NBC igather 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 igather"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-NBC igather_pure 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 igather_pure"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-NBC igather_pure 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 igather_pure"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-NBC igatherv 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 igatherv"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-NBC igatherv 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 igatherv"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-NBC igatherv_pure 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 igatherv_pure"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-NBC igatherv_pure 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 igatherv_pure"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-NBC ireduce 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 ireduce"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-NBC ireduce 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 ireduce"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-NBC ireduce_pure 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 ireduce_pure"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-NBC ireduce_pure 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 ireduce_pure"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-NBC ireduce_scatter 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 ireduce_scatter"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-NBC ireduce_scatter 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 ireduce_scatter"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-NBC iscatter 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 iscatter"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-NBC iscatter 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 iscatter"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-NBC iscatter_pure 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 iscatter_pure"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-NBC iscatter_pure 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 iscatter_pure"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-NBC iscatterv 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 iscatterv"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-NBC iscatterv 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 iscatterv"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-NBC iscatterv_pure 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 iscatterv_pure"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-NBC iscatterv_pure 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-NBC -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 iscatterv_pure"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-MPI1 reduce 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 reduce"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-MPI1 reduce 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 reduce"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-MPI1 reduce_scatter 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 reduce_scatter"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-MPI1 reduce_scatter 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 reduce_scatter"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-MPI1 reduce_scatter_block 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 reduce_scatter_block"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-MPI1 reduce_scatter_block 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 reduce_scatter_block"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-MPI1 allreduce 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 allreduce"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-MPI1 allreduce 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 allreduce"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-MPI1 allgather 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 allgather"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-MPI1 allgather 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 allgather"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-MPI1 allgatherv 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 allgatherv"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-MPI1 allgatherv 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 allgatherv"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-MPI1 scatter 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 scatter"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-MPI1 scatter 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 scatter"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-MPI1 scatterv 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 scatterv"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-MPI1 scatterv 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 scatterv"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-MPI1 gather 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 gather"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-MPI1 gather 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 gather"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-MPI1 gatherv 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 gatherv"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-MPI1 gatherv 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 gatherv"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-MPI1 alltoall 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 alltoall"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-MPI1 alltoall 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 alltoall"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-MPI1 bcast 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 bcast"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-MPI1 bcast 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 bcast"
        [ "$status" -eq 0 ]
}
# RC
@test "IMB-MPI1 barrier 20 ranks, 5 ranks per node using RC verbs" {
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 barrier"
        [ "$status" -eq 0 ]
}

# XRC
@test "IMB-MPI1 barrier 20 ranks, 5 ranks per node using XRC verbs" {
        eval ${XRC_FI_ENV} \
        run $CONTRIB_BIN/logwrap -w ${BATS_TEST_LOGFILE} -- \
                $(batch_launcher 20 5) timeout 300 "$IMB_BUILD_PATH/IMB-MPI1 -npmin 20 -iter 100 -time 10 -mem 2 -msglog 2:18 barrier"
        [ "$status" -eq 0 ]
}
