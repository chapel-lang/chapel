@ECHO OFF
REM Test default configuration against full suite on windows.

REM WORKSPACE is always set inside our testing environment. If this is run
REM outside that environment, the caller is responsible for setting it.
IF "%WORKSPACE%"=="" GOTO ErrExit

REM NOTE: This is pretty messy, but it is the only way I could figure out how to
REM       get the correct environment setup and then invoke
REM       nightly. (thomasvandoren, 2014-07-14)
c:\cygwin\bin\bash -exc "export PATH='/usr/local/bin:/usr/bin:/cygdrive/c/Windows/system32:/cygdrive/c/Windows:/cygdrive/c/Windows/System32/Wbem:/cygdrive/c/Program Files/SysinternalsSuite' ; export CHPL_TEST_LIMIT_RUNNING_EXECUTABLES=yes ; export CHPL_HOME=$PWD ; num_procs=`python -c 'import multiprocessing; print (multiprocessing.cpu_count()+1)/2'` ; for i in $(seq 1 $num_procs); do echo localhost; done > $CHPL_HOME/test/NODES/$num_procs-localhost ; export CHPL_UTIL_SMTP_HOST=relaya ; source $CHPL_HOME/util/cron/common.bash && export CHPL_NIGHTLY_TEST_CONFIG_NAME="cygwin32" && $CHPL_HOME/util/cron/nightly -cron -parnodefile $CHPL_HOME/test/NODES/$num_procs-localhost"
GOTO End

:ErrExit
ECHO "ERROR: WORKSPACE must be set in the environment"
EXIT 1
GOTO End

:End
