.. _readme-nightlytesting:

===============
Nightly Testing
===============

This is intended to help get external developers running nightly testing. The
following script should be called from a cron job or something to kick off the
testing regularly. This script basically groups all the env vars you'll need to
set into one place and handles downloading the chapel repo for testing. It then
calls our `nightly` script which runs a set of tests and mails results to a
provided list. It keeps these nightly logs so it can track new or resolved
regressions. The nightly script basically builds the compiler and runtime, runs
some tests and parses the test output to send mails. By default it will do a
correctness run, though it can also be configured to test performance, compiler
performance, memleaks, and several other configurations.

In the following script you'll need to set whatever special CHPL_* vars are
required for your testing and at a minimum `CHPL_NIGHTLY_CRON_RECIPIENT` and
`CHPL_NIGHTLY_TEST_CONFIG_NAME`. There are additional configuration variables
that can be set, but they have pretty sane defaults so they can probably be
left alone.

To test this script without running the entire test suite, you can simply run
this script with ``-hellos``. i.e. if the script is named "test_chapel" then::

    ./test_chapel -hellos

will only test the 6 hello*.chpl examples instead of the whole suite. If that
is successful the script can simply be called without any args and it will do a
correctness test on the entire chapel test suite::

    ./test_chapel

.. code-block:: sh

    #!/usr/bin/env bash

    # This is a script to help external developers run nightly testing. It
    # essentially downloads a copy of the chapel repo and sets some required
    # variables and then runs our "nightly" script. Any arguments are passed
    # along to the nightly script. See $CHPL_HOME/util/cron/nightly for
    # available options.


    #
    # Env vars required for testing a specific config. For example:
    #   export CHPL_TASKS=<MY_AWESOME_TASKING_LAYER>
    #


    #
    # Config vars that need to be set
    #

    # Change to appropriate mailing list or email
    export CHPL_NIGHTLY_CRON_RECIPIENT=email.example.com

    # Config name used in mail and log file names
    export CHPL_NIGHTLY_TEST_CONFIG_NAME="chapel.test"


    #
    # Config vars with probably safe defaults
    #

    # Used to store permanent log files (and to diff runs night to night)
    export CHPL_NIGHTLY_LOG_PREFIX=~/chapel_logs/

    # Used to explicitly set a tmp dir. This is where chapel is downloaded
    # to and tested from. Defaults to $TMPDIR if set, otherwise /tmp
    export CHPL_NIGHTLY_TMPDIR=

    # Used to select which git repo and branch to clone
    export CHPL_NIGHTLY_GIT_URL="https://github.com/chapel-lang/chapel.git"
    export CHPL_NIGHTLY_GIT_BRANCH="main"


    #
    # helper logging routines
    #

    function log_fatal() {
        local msg=$@
        echo "[FATAL] ${msg}"
        exit 1
    }

    function log_info() {
        local msg=$@
        echo "[INFO] ${msg}"
    }


    #
    # ensure that git is available
    #

    git --version &>/dev/null
    if [ $? -eq 0 ]; then
      log_info "git is installed"
    else
      log_fatal "git is required to use this script"
    fi


    #
    # download the chapel repo if it does not exist, otherwise just clean
    # the existing one and get it up to date
    #

    tmp_dir=${CHPL_NIGHTLY_TMPDIR:-${TMPDIR:-/tmp}}
    export CHPL_NIGHTLY_TMPDIR="$tmp_dir"
    chapel_dir="$tmp_dir/chapel"

    if [ -d "$chapel_dir" ]; then
      log_info "chapel dir exists at $chapel_dir, cleaning repo and then pulling updated sources"
      cd "$chapel_dir"
      git clean -fdx . && git pull
      if [ $? -eq 0 ]; then
        log_info "Successfully cleaned and updated sources"
      else
        log_fatal "Failed to clean and update sources"
      fi
    else
      log_info "chapel dir does NOT exist at $chapel_dir, cloning a copy of the repo"
      git clone --branch "$CHPL_NIGHTLY_GIT_BRANCH" "$CHPL_NIGHTLY_GIT_URL" "$chapel_dir"
      if [ $? -eq 0 ]; then
        log_info "Successfully cloned repo"
      else
        log_fatal "Failed to clone repo"
      fi
    fi


    #
    # Let's do some testing!
    #

    # setup chpl_home and stuff
    source "$chapel_dir/util/setchplenv.bash"

    # source the common nightly testing script to get common vars
    cd "$chapel_dir/util/cron"
    source common.bash

    # Call our nightly script forwarding any args passed into this script
    $CWD/nightly -cron "$@"

