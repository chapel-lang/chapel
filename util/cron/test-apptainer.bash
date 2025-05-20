#!/usr/bin/env bash
#
# This script will build and run chapel in different linux distros using apptainer.
# For detailed info please refer README located at util/devel/test/apptainer/README.md

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash

function do_cleanup {
  log_info "Cleaning up Chapel checkout and apptainer images"
  ./chapel-delete-all.sh
  ./image-delete-all.sh
}

function do_cleanup_and_exit {
  do_cleanup
  exit 1
}

export CHPL_HOME=$(cd $UTIL_CRON_DIR/../.. ; pwd)
log_info "Setting CHPL_HOME to: ${CHPL_HOME}"

cd $CHPL_HOME/util/devel/test/apptainer

./chapel-quickstart-delete.sh
if [ $? -ne 0 ]
   then
     log_error "./chapel-quickstart-delete.sh exited with error"
      do_cleanup_and_exit
   else
     log_info "./chapel-quickstart-delete.sh succeeded"
fi

# Commented out to just test chapel-quickstart in Jenkins.
# Will add this once we know what configurations will fail(most/all of the non-llvm) have the logic to fail the test 
# ./chapel-default.sh
# if [ $? -ne 0 ] 
#    then
#      log_error "./chapel-default.sh exited with error" 
#      do_cleanup_and_exit
#    else
#      log_info "./chapel-default.sh succeeded"  
#    fi 

do_cleanup

export CHPL_NIGHTLY_TEST_CONFIG_NAME="apptainer"
