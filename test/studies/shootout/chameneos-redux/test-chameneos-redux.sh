# usage:    to be run with the command: sh test-chameneos-redux.sh
# output:   runManyTimes.out should contain the word "halt" only if a chameneos was 
#   	    allowed to meet with itself, otherwise should be empty

chpl -o chameneos-redux $CHPL_HOME/test/studies/shootout/chameneos-redux/chameneos-redux.chpl
$CHPL_HOME/util/devel/runManyTimes 1 1000 ./chameneos-redux --N=20
grep halt runManyTimes.out > runManyTimes.out
