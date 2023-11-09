#!/bin/bash

# Script to compare performance of the original parflow against the chapel
#  interop variant
#
# Arguments: Number of repetitions to run, default 5.
#
# Assumptions:
#   PARFLOW_DIR is set to the parflow base directory,
#   CHPL_HOME is set to the chapel base directory
#   Others to come
#
# Outputs:
#   performance-evaluation.results: execution times for original and chapel
#       variants. Each line contains the variant ("original" or "chapel") and
#       a comma separated list of the execution times. Example: original, 1.2, 1.3



LAUNCH_DIR=$(pwd)



CMAKE_ORIGINAL="cmake ${CMAKE_DEFS} .."

CMAKE_CHAPEL="${CMAKE_ORIGINAL} -DPARFLOW_HAVE_CHAPEL=ON"


echo "Reading command line arguments..."

LW="LW_Tiny.tcl"
RUN_ORIGINAL=0
RUN_CHAPEL=0
RUN_CHAPEL_FAST=0
APEEND=0
RESULT_NAME=performance-evaluation.results
CHECK_CORRECTNESS=0
NUMRUNS=2
while test $# -gt 0
do
    case "$1" in
        --original) echo "Running original variant";
            RUN_ORIGINAL=1
            ;;
        --omp) echo "Running openmp variant";
            RUN_OMP=1;
            ;;
        --cuda) echo "Running cuda variant";
            RUN_CUDA=1;
            ;;
        --chapel) echo "Running chapel variant";
            RUN_CHAPEL=1
            ;;
        --chapel-fast) echo "Running chapel fast variant";
            RUN_CHAPEL_FAST=1
            ;;
        --append) echo "Appending to results file";
            APPEND=1
            ;;
        --outfile) echo "Writing to $2";
            RESULT_NAME=$2
            shift
            ;;
        --n) echo "running $2 times";
            NUMRUNS=$2; shift
            ;;
        --check-correctness) echo "Checking correctness for executed variants.";
            CHECK_CORRECTNESS=1
            ;;
        --lw) echo "Using LW input file $2"
            LW=$2
            shift
            ;;
        --new-correct) echo "Reseting correctness checking file."
            NEW_CORRECT=1
            ;;
        *) echo "argument $1"
            exit
            ;;
    esac
    shift
done
RUNSEQ=$(seq 1 $NUMRUNS)


# Configures the parflow project using CMake. Supports optional arguments for 
#  configuring with chapel (--chapel) and openmp (--omp). Configures for 
#  correctness checks based on the CHECK_CORRECTNESS variable

configure_parflow() {
    pushd $PARFLOW_DIR/build
    CMAKE_DEFS="-DPARFLOW_HAVE_CLM=ON -DCMAKE_INSTALL_PREFIX=${PARFLOW_DIR} \
    -DSILO_ROOT=${SILO_DIR} -DHYPRE_ROOT=${HYPRE_DIR} \
    -DPARFLOW_AMPS_LAYER=mpi1 -DCMAKE_BUILD_TYPE=RELWITHDEBINFO \
    -DPARFLOW_ENABLE_TIMING=true"

    while test $# -gt 0
    do
        case "$1" in
            --chapel) CMAKE_DEFS="$CMAKE_DEFS -DPARFLOW_HAVE_CHAPEL=ON"
                ;;
            --omp) CMAKE_DEFS="$CMAKE_DEFS -DPARFLOW_HAVE_OMP=ON"
                ;;
        esac
        shift
    done

    if [[ $CHECK_CORRECTNESS -eq 1 ]]; then
        CMAKE_DEFS="$CMAKE_DEFS -DPARFLOW_CHECK_CORRECTNESS=ON"
    fi

    if [[ $NEW_CORRECT -eq 1 ]]; then
        CMAKE_DEFS="$CMAKE_DEFS -DPARFLOW_CHECK_CORRECTNESS=ON"
    fi
    
    rm -rf ./*
    
    if cmake $CMAKE_DEFS .. ; then 
        echo "Configure successful."
    else
        echo "Configure failed."
        echo "Configure Command: " cmake $CMAKE_DEFS ..
        exit 3
    fi
    popd
}
#
#Functions
#
build_parflow() {
    pushd $PARFLOW_DIR/build
    BP_CMD="make -j16"

    if $BP_CMD ; then
        echo "Build successful."
    else
        echo "Build failed."
        exit 2
    fi

    if make install ; then
        echo "Install successful."
    else
        echo "Install failed."
        exit 2
    fi
    popd
}


build_chapel_modules() {
    pushd $PARFLOW_DIR/pfsimulator/chapel
    BCM_FAST=0

    while test $# -gt 0
    do
        case "$1" in
            --fast) BCM_FAST=1;
                ;;
        esac
        shift
    done

    BCM_INCLUDES="-I../parflow_lib -I../parflow_lib \
    -I ../amps/mpi1/ -I../../build/include/ -I../amps/common -I/usr/include/aarch64-linux-gnu/mpi"

    BCM_COMMAND="chpl $BCM_INCLUDES"
    if [[ $BCM_FAST -eq 1 ]]; then
        BCM_COMMAND="$BCM_COMMAND --fast "
    fi

    BCM_COMMAND="$BCM_COMMAND --library-cmakelists ChapelImpl.chpl "

    if $BCM_COMMAND; then
        echo "Chapel modules built successfully."
    else
        echo "Chapel module failed to build."
        exit
    fi
    popd
}

check_correctness() {
    if [[ $# -eq 1 ]]; then
        CC_PREFIX=$1_
    fi
    DIFF_FILE=$LAUNCH_DIR/${CC_PREFIX}correctness_diff
    echo "Checking correctness..."
    pushd $PARFLOW_DIR/test/tcl/washita/tcl_scripts

    echo "failed" > $DIFF_FILE
    echo "Running diff..."
    diff Outputs/cp.out correct_output/cp.out > $DIFF_FILE
    echo "Checking diff output..."
    DIFF=$(head $DIFF_FILE)
    if [ "$DIFF" ]; then
        echo "Correctness check failed."
        exit 4
    else
        echo "Correctness check passed."
    fi
    popd
}

#Runs the example and saves the execution times. Name of the variant is
# the first and only argument to this function
run_example() {
    RE_VARIANT_NAME=$1

    pushd $PARFLOW_DIR/test/tcl/washita/tcl_scripts

    echo -n "${RE_VARIANT_NAME}," >> $RESULTS_FILE

    for i in $RUNSEQ
    do
        rm Outputs/cp.out
        echo "${RE_VARIANT_NAME}, run $i"
        tclsh $LW

        RESULT_LINE="$(grep "Total Run Time" Outputs/LW.out.log)"
        RESULT_ARRAY=($RESULT_LINE)
        EXEC_TIME=${RESULT_ARRAY[3]}
        echo $EXEC_TIME
        echo -n " $EXEC_TIME," >> $RESULTS_FILE
    done
    echo >> $RESULTS_FILE

    if [[ $CHECK_CORRECTNESS -eq 1 ]]; then
        check_correctness $RE_VARIANT_NAME
    fi
    popd
}


#
#Actual Script
#
echo "Creating results file..."
RESULTS_FILE=$LAUNCH_DIR/$RESULT_NAME
if [[ $APPEND -eq 0 ]]; then
    rm $RESULTS_FILE
fi
touch $RESULTS_FILE

echo "Creating build directory..."
cd $PARFLOW_DIR
mkdir build



run_original() {
    echo "Running original variant."

    cd $PARFLOW_DIR/build
    echo "Clearing build directory..."
    rm -rf ./*

    echo "Configuring..."
    configure_parflow

    echo "Building..."
    build_parflow

    echo "Preparing to run Little Washita Example..."
    run_example original

    echo "Done running original variant."
}

if [[ $NEW_CORRECT -eq 1 ]]; then
    echo "Resetting correctness check data..."
    run_original
    echo "Done running original, copying check file..."
    pushd $PARFLOW_DIR/test/tcl/washita/tcl_scripts

    cp Outputs/cp.out correct_output/cp.out
    echo "Done copying check file."
    popd
fi

if [[ $RUN_ORIGINAL -eq 1 ]]; then
    run_original
fi

run_omp() {

    echo "Running OpenMP Variant"

    cd $PARFLOW_DIR/build
    echo "Clearing build directory..."
    rm -rf ./*

    echo "Configuring..."
    configure_parflow --omp

    echo "Building and Installing"
    build_parflow

    echo "Preparing to run Little Washita Example..."
    run_example openmp
}

if [[ $RUN_OMP -eq 1 ]]; then
    run_omp
fi

run_chapel() {
    echo
    echo "Running Basic Chapel Variant."

    echo "Preparing chapel modules..."
    build_chapel_modules


    echo "Clearing build for chapel variant..."
    cd $PARFLOW_DIR/build
    rm -rf ./*

    echo "Configuring..."
    configure_parflow --chapel

    echo "Building and Installing"
    build_parflow

    echo "Preparing to run Little Washita Example..."
    run_example "chapel"
}

if [[ $RUN_CHAPEL = 1 ]]; then
    run_chapel
fi 

run_chapel_fast() {
    echo "Running Chapel --fast Variant"

    echo "Preparing chapel modules..."
    build_chapel_modules --fast


    echo "Configuring..."
    configure_parflow --chapel

    echo "Building..."
    build_parflow

    echo "Preparing to run Little Washita Example..."
    run_example "chapel-fast"
}

if [[ $RUN_CHAPEL_FAST = 1 ]]; then
    run_chapel_fast
fi
