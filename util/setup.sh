#!/bin/bash

# Set the CONFIGFILE to be whatever's in the first argument.
# If no argument is passed in, then simply use the file CONFIG.
if [ -z "$1" ]; then
    CONFIGFILE=$CHPL_HOME/util/CONFIG
    FLAG=""
else
    FIRSTCHARACTER=`echo $1 | cut -c1`
    if [ x"$FIRSTCHARACTER" = x"-" ]; then
        CONFIGFILE=$CHPL_HOME/util/CONFIG
        FLAG=$@
    elif [ -r "$1" ]; then
        ISCONFIGFILE=`cat $1 | grep "HOST" | awk -F \= '{print $1}' | grep "HOST"`
        if [ -n "$ISCONFIGFILE" ]; then
            CONFIGFILE=$1
            shift
            FLAG=$@
        else
            CONFIGFILE=$CHPL_HOME/util/CONFIG
            FLAG=$@
        fi
    else
        CONFIGFILE=$CHPL_HOME/util/CONFIG
        FLAG=$@
    fi
fi

if [ ! -r "$CONFIGFILE" ]; then
    echo "Make sure config file exists and is readable."
    echo "Usage: $0 <config-file> <flags>"
    echo "       $0 <flags> (if file called $CHPL_HOME/util/CONFIG exists)"
    exit 1
fi

# Parse through the arguments to find things to compile and additional flags.
# Exclude output files (script provides them anyway).
HASCOMPILE=`echo "${FLAG}" | grep "\-o"`
TESTLIST=""
if [ -n "$HASCOMPILE" ]; then
    FLAG=""
    SKIPNEXT=0
    for arg in "$@"; do
        FIRSTCHARACTER=`echo $arg | cut -c1`
        SECONDCHARACTER=`echo $arg | cut -c2`
        ISCHPL=`echo $arg | grep "\.chpl"`
        if [ x"$SKIPNEXT" = x"1" ]; then
            SKIPNEXT=0
        elif [ x"$FIRSTCHARACTER" = x"-" -a x"$SECONDCHARACTER" = x"o" ]; then
            THIRDCHARACTER=`echo $arg | cut -c3`
            if [ -z "$THIRDCHARACTER" ]; then
                SKIPNEXT=1
            fi
        elif [ -z "$ISCHPL" ]; then
            FLAG="$FLAG $arg"
        else
            TESTLIST="$TESTLIST $arg"
        fi
    done
fi

# Capture relevant information into lists from the configuration file.
PLATFORMLIST=`cat ${CONFIGFILE} | grep "PLATFORM" | awk -F \= '{print $2}'`
HOSTLIST=`cat ${CONFIGFILE} | grep "HOST" | awk -F \= '{print $2}'`
PATHLIST=`cat ${CONFIGFILE} | grep "PATH" | awk -F \= '{print $2}'`
FLAG=$FLAG" "`cat ${CONFIGFILE} | grep "FLAGS" | sed 's/FLAGS=//'`
TESTLIST=$TESTLIST" "`cat ${CONFIGFILE} | grep "TEST" | awk -F \= '{print $2}'`

if [ -z "$HOSTLIST" ]; then
    echo "Define some hosts in $CONFIGFILE"
    echo "Typical configuration is as follows:"
    echo "     PLATFORM1=$CHPL_HOST_PLATFORM"
    echo "     HOST1=`uname -n`"
    echo "     PATH1=$CHPL_HOME/bin/$CHPL_HOST_PLATFORM"
    echo ""
    echo "     PLATFORM2=darwin"
    echo "     ..."
    echo "     FLAGS=-snumRealms=2"
    echo "     TEST1=test/release/examples/hello.chpl"
    echo "     ..."
    exit 1
fi

# make Chapel sources and tests defined in configuration file on all nodes.
j=1
for host in ${HOSTLIST}; do
    platform=`echo ${PLATFORMLIST} | awk '{print $'$j'}'`
    path=`echo ${PATHLIST} | awk '{print $'$j'}'`

    if [ x"$host" = x`uname -n` ]; then
        touch /tmp/Chplpvmtmp
        rm -rf /tmp/*pvm*
        killall -9 pvmd3
        # Comment out the following two lines if you don't want to make
        # the Chapel sources.
        make depend
        make
        for test in ${TESTLIST}; do
            name=`echo ${test} | awk -F \/ '{print $NF}' | awk -F . '{print $1}'`
            chpl $FLAG -o ${path}/${name}_${platform} ${CHPL_HOME}/${test}
        done
    else
        CMD='env | grep SHELL | awk -F \= '"'"'{print $2}'"'"
        RSHELL="$(ssh $host "$CMD")"
        if [ x"${RSHELL}" = x/bin/tcsh -o x"${RSHELL}" = x/bin/csh ]; then
            # Comment out the make depend and make line if you don't want to
            # make the Chapel sources remotely.
            ssh ${host} "setenv CHPL_HOME ${CHPL_HOME}; setenv CHPL_COMM ${CHPL_COMM}; setenv CHPL_DEVELOPER ${CHPL_DEVELOPER}; touch /tmp/Chplpvmtmp; rm -rf /tmp/*pvm*; killall -9 pvmd3; cd ${CHPL_HOME}; 
                make depend; make
                "
            for test in ${TESTLIST}; do
                name=`echo ${test} | awk -F \/ '{print $NF}' | awk -F . '{print $1}'`
                ssh ${host} "setenv CHPL_HOME ${CHPL_HOME}; setenv CHPL_COMM ${CHPL_COMM}; setenv CHPL_DEVELOPER ${CHPL_DEVELOPER}; ${path}/chpl $FLAG -o ${path}/${name}_${platform} ${CHPL_HOME}/${test}"
            done
        elif [ x"${RSHELL}" = x/bin/ksh -o x"${RSHELL}" = x/bin/bash ]; then
            # Comment out the make depend and make line if you don't want to
            # make the Chapel sources remotely.
            ssh ${host} "export CHPL_HOME=${CHPL_HOME}; export CHPL_COMM=${CHPL_COMM}; export CHPL_DEVELOPER=${CHPL_DEVELOPER}; touch /tmp/Chplpvmtmp; rm -rf /tmp/*pvm*; killall -9 pvmd3; cd ${CHPL_HOME};
                make depend; make
                "
            for test in ${TESTLIST}; do
                name=`echo ${test} | awk -F \/ '{print $NF}' | awk -F . '{print $1}'`
                ssh ${host} "export CHPL_HOME=${CHPL_HOME}; export CHPL_COMM=${CHPL_COMM}; export CHPL_DEVELOPER=${CHPL_DEVELOPER}; ${path}/chpl $FLAG -o ${path}/${name}_${platform} ${CHPL_HOME}/${test}"
            done
        else
            echo "Unknown shell $RSHELL on $host."
        fi
    fi

    j=$(($j+1))
done
