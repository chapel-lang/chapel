#!/usr/bin/env bash
#
# This script will build and run chapel in different linux distros using apptainer. 
# For detailed info please refer README located at util/devel/test/apptainer/README.md

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

export CHPL_HOME=$(cd $CWD/../.. ; pwd)
log_info "Setting CHPL_HOME to: ${CHPL_HOME}"

cd $CHPL_HOME/util/devel/test/apptainer

./chapel-quickstart-delete.sh

if [ $? -ne 0 ] 
   then
     log_error "./chapel-quickstart-delete.sh exited with error" 
     log_info "cleaning before exiting"
     ./chapel-delete.sh 
     ./image-delete.sh
     exit 1
   else
     log_info "./chapel-quickstart-delete.sh succeeded"  
fi 

# Commented out to just test chapel-quickstart in Jenkins.
# Will add this once we know what configurations will fail(most/all of the non-llvm) have the logic to fail the test 
# if [ $? -ne 0 ] 
#    then
#      log_error "./chapel-default.sh exited with error" 
#      exit 1
#    else
#      log_info "./chapel-default.sh succeeded"  
#    fi 

./chapel-delete.sh 
./image-delete.sh

export CHPL_NIGHTLY_TEST_CONFIG_NAME="apptainer"
