# csh/tcsh shell script to set the Chapel environment variables

# shallow test to see if we are in the correct directory
set path_tail = `echo $cwd | sed 's/.*\///g'`
if ($path_tail != "chapel") then
   echo "Error: source util/setchplenv from within the chapel directory"
   exit
endif

setenv CHPL_HOME $cwd
setenv CHPL_PLATFORM `$CHPL_HOME/util/platform`
setenv PATH $PATH":"$CHPL_HOME"/bin/"$CHPL_PLATFORM
