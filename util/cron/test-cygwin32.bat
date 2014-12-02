@ECHO OFF
REM Test default configuration against full suite on windows.

REM WORKSPACE is always set inside our testing environment. If this is run
REM outside that environment, the caller is responsible for setting it.
IF "%WORKSPACE%"=="" GOTO ErrExit

REM NOTE: This is pretty messy, but it is the only way I could figure out how to
REM       get the correct environment setup and then invoke
REM       nightly. (thomasvandoren, 2014-07-14)
c:\cygwin\bin\bash -exc "export PATH='FIXME' ; export CHPL_HOME=$PWD ; export CHPL_MAILER=email ; source $CHPL_HOME/util/cron/common.bash && export CHPL_NIGHTLY_TEST_CONFIG_NAME="cygwin32" && $CHPL_HOME/util/cron/nightly -cron -no-futures"
GOTO End

:ErrExit
ECHO "ERROR: WORKSPACE must be set in the environment"
EXIT 1
GOTO End

:End
