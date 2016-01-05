@ECHO OFF
REM Test default configuration against full suite on windows.

REM WORKSPACE is always set inside our testing environment. If this is run
REM outside that environment, the caller is responsible for setting it.
IF "%WORKSPACE%"=="" GOTO ErrExit

REM NOTE: This is pretty messy, but it is the only way I could figure out how to
REM       get the correct environment setup and then invoke
REM       nightly. (thomasvandoren, 2014-07-14)
c:\cygwin\bin\bash -exc "export PATH='/usr/local/bin:/usr/bin:/cygdrive/c/ProgramData/Oracle/Java/javapath:/cygdrive/c/Windows/system32:/cygdrive/c/Windows:/cygdrive/c/Windows/System32/Wbem:/cygdrive/c/Windows/System32/WindowsPowerShell/v1.0:/cygdrive/c/Program Files/SysinternalsSuite:/cygdrive/c/Program Files/emacs-24.4/bin' ; export CHPL_HOME=$PWD ; export CHPL_UTIL_SMTP_HOST=relaya ; source $CHPL_HOME/util/cron/common.bash && export CHPL_NIGHTLY_TEST_CONFIG_NAME="cygwin32" && $CHPL_HOME/util/cron/nightly -cron"
GOTO End

:ErrExit
ECHO "ERROR: WORKSPACE must be set in the environment"
EXIT 1
GOTO End

:End
