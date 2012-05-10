#!/bin/bash

# Alternative parallel test script. util/test/paratest.server is the main one.

CLEAN=../svn_trunk_clean
# note that DST and CLEANDST MUST BE DIFFERENT from .
# they WILL BE DELETED.
DST=/tmp/`whoami`_svn_trunk
CLEANDST=/tmp/`whoami`_svn_trunk_clean
SEED=seed-run-test.dat
NODESFILE=nodefile
TESTSPER=8
MAXLOAD=80%
MAXJOBS=50%

ABSSEED=`echo $SEED | perl -e 'use File::Spec; while(<>) { chomp; $p = File::Spec->rel2abs($_); print "$p\n"; }'`

if [ -z $ABSSEED ]
then
  ABSSEED=/tmp/seed-run-test.dat
fi

if [ ! -f $SEED ]
then
  head -c 65536 /dev/urandom > $SEED
fi


# Make sure we have basic environment.
unset CHPL_DEVELOPER
unset CHPL_MEM
unset CHPL_TASKS
unset CHPL_COMM
unset CHPL_ATOMICS

findtests () {
  IGNORE=".svn Logs Samples Share perfdat Bin OUTPUT RCS"
  IGNORES=""
  for dir in $IGNORE
  do
    IGNORES="$IGNORES -name $dir -prune -o"
  done
  PRUNES=""
  for dir in `find . -name sub_test -printf "%h\n"` 
  do 
    PRUNES="$PRUNES -path $dir -prune -print -o"
  done
  find . $IGNORES $PRUNES -name '*.chpl' -exec sh -c 'C="{}"; G="${C%.chpl}.good"; [ -f $G -a ! -f NOTEST ]' \; -print | shuf --random-source $ABSSEED -o .tests.txt
  return $?;
#../util/start_test -clean-only | grep "\[pwd:" |  cut -d ' ' -f 2- | cut -d ']' -f 1 | perl -e 'use File::Spec; while(<>) { chomp; $p = File::Spec->abs2rel($_); print "$p\n"; }' > .tests.txt
}

# Clean up and prepare.
pushd $CLEAN || { echo 'could not pushd $CLEAN - you should have a clean svn/trunk checkout there, or you can update CLEAN in this script' ; exit 1; }
source util/setchplenv.bash > /dev/null
echo MAKING $CLEAN
#make clean > /dev/null
make > /dev/null || { echo 'make failed' ; exit 1; }
make spectests > /dev/null || { echo 'make failed' ; exit 1; }
echo FINDING TESTS in $CLEAN/test
cd test
findtests
popd
pushd .
source util/setchplenv.bash > /dev/null
echo MAKING .
#make clean > /dev/null
make > /dev/null || { echo 'make failed' ; exit 1; }
make spectests > /dev/null || { echo 'make failed' ; exit 1; }
echo FINDING TESTS in ./test
cd test
findtests
popd

# Now, on each target node, rsync
echo RSYNCING the data to the remote nodes
cat $NODESFILE | parallel rsync -az --exclude ".svn/" --delete ./ {}:$DST
cat $NODESFILE | parallel rsync -az --exclude ".svn/" --delete ../svn_trunk_clean/ {}:$CLEANDST

# Now, run the clean tests.
cat $CLEAN/test/.tests.txt | parallel --jobs $MAXJOBS --load $MAXLOAD --eta --sshloginfile $NODESFILE -L $TESTSPER "cd $CLEANDST; source util/setchplenv.bash > /dev/null; cd test; ../util/start_test -logfile Logs/run-test-{#} -logtmp  Logs/run-test-tmp-{#} -norecurse {} > /dev/null; grep '\[Error' Logs/run-test-{#}.summary" > $CLEAN/test/Logs/run-test.log
cat ./test/.tests.txt | parallel --jobs $MAXJOBS --load $MAXLOAD --eta --sshloginfile $NODESFILE -L $TESTSPER "cd $DST; source util/setchplenv.bash > /dev/null; cd test; ../util/start_test -logfile Logs/run-test-{#} -logtmp Logs/run-test-tmp-{#} -norecurse {} > /dev/null; grep '\[Error' Logs/run-test-{#}.summary" >  test/Logs/run-test.log

sort $CLEAN/test/Logs/run-test.log > $CLEAN/test/Logs/run-test.log.sorted
sort ./test/Logs/run-test.log > ./test/Logs/run-test.log.sorted

grep -v '^Future' $CLEAN/test/Logs/run-test.log.sorted > $CLEAN/test/Logs/run-test.log.sorted.nofutures
grep -v '^Future' ./test/Logs/run-test.log.sorted > ./test/Logs/run-test.log.sorted.nofutures

comm -1 -3 $CLEAN/test/Logs/run-test.log.sorted.nofutures ./test/Logs/run-test.log.sorted.nofutures

