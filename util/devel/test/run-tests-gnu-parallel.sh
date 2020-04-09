#!/bin/bash

# Alternative parallel test script. util/test/paratest.server is the main one.
COMM=none
CLEAN=../svn_trunk_clean
# note that DST and CLEANDST MUST BE DIFFERENT from .
# they WILL BE DELETED.
DST=/scratch/`whoami`_svn_trunk
CLEANDST=/scratch/`whoami`_svn_trunk_clean
SEED=seed-run-test.dat
NODESFILE=nodefile
TESTSPER=8
MAXLOAD=80%
MAXJOBS=50%
RETRY=0
ABSSEED=`echo $SEED | perl -e 'use File::Spec; while(<>) { chomp; $p = File::Spec->rel2abs($_); print "$p\n"; }'`

USAGE="Usage: `basename $0` [-c gasnet|none] [-r]\n Run this from the CHPL_HOME you wish to test\n -c sets comm layer\n -r retries only tests that failed";

while getopts hrc: OPT; do
  case "$OPT" in
    h)
      echo $USAGE
      exit 0
      ;;
    r)
      RETRY=1
      ;;
 
    c)
      COMM=$OPTARG
      ;;
    \?)
      echo $USAGE >&2
      exit 1
      ;;
  esac
done

if [ "$COMM" != "none" ]
then
  MAXJOBS=1
fi

echo Testing with comm=$COMM clean=$CLEAN seed=$SEED nodesfile=$NODESFILE cores=$MAXJOBS clean=$CLEAN dst=$DST cleandst=$CLEANDST

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
export CHPL_COMM=$COMM
# set things used by skipif files.
export CHPL_HOST_PLATFORM=`$CHPL_HOME/util/chplenv/chpl_platform.py --host`
export CHPL_TARGET_PLATFORM=`$CHPL_HOME/util/chplenv/chpl_platform.py --target`
export CHPL_HOST_COMPILER=`$CHPL_HOME/util/chplenv/chpl_compiler.py --host`
export CHPL_TARGET_COMPILER=`$CHPL_HOME/util/chplenv/chpl_compiler.py --target`
export CHPL_TASKS=`$CHPL_HOME/util/chplenv/chpl_tasks.py`
export CHPL_COMM=`$CHPL_HOME/util/chplenv/chpl_comm.py`
export CHPL_GMP=`$CHPL_HOME/util/chplenv/chpl_gmp.py`
export CHPL_HWLOC=`$CHPL_HOME/util/chplenv/chpl_hwloc.py`


findtestsretry () {
  echo FINDING TESTS to RETRY
  comm -1 -3 $CLEAN/test/Logs/run-test.log.sorted.nofutures test/Logs/run-test.log.sorted.nofutures |  perl -e 'while(<>) { if(m/ ((\w+\/)+\w+)[\] ]/) { print $1 . ".chpl\n"} }' > test/.tests.txt
  return $?;
}

findtests () {
  IGNORE=".svn Logs Samples perfdat Bin OUTPUT RCS"
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
  find . $IGNORES $PRUNES -name '*.chpl' -execdir sh -c 'C="{}"; G="${C%.chpl}.good"; P="${C%.chpl}.prediff"; S="${C%.chpl}.skipif"; [ -f $G -o -f $P ] && [ ! -f TIMEOUT ] && [ ! -f NOTEST ] && $CHPL_HOME/util/devel/test/checkIgnore.sh $S' \; -print | sort -o .tests-no-timeout-sorted.txt
  find . $IGNORES $PRUNES -name '*.chpl' -execdir sh -c 'C="{}"; G="${C%.chpl}.good"; P="${C%.chpl}.prediff"; S="${C%.chpl}.skipif"; [ -f $G -o -f $P ] && [ -f TIMEOUT ] && [ ! -f NOTEST ] && $CHPL_HOME/util/devel/test/checkIgnore.sh $S' \; -print | sort -o .tests-timeout-plus-sorted.txt

  comm -1 -3 .tests-no-timeout-sorted.txt .tests-timeout-plus-sorted.txt > .tests-timeout-sorted.txt
  shuf --random-source $ABSSEED -o .tests-timeout.txt .tests-timeout-sorted.txt
  shuf --random-source $ABSSEED -o .tests-no-timeout.txt .tests-no-timeout-sorted.txt
  cat .tests-timeout.txt .tests-no-timeout.txt > .tests.txt
}

# Clean up and prepare.
pushd $CLEAN || { echo 'could not pushd $CLEAN - you should have a clean svn/trunk checkout there, or you can update CLEAN in this script' ; exit 1; }
source util/setchplenv.bash > /dev/null
echo MAKING $CLEAN
#make clean > /dev/null
make > /dev/null || { echo 'make failed' ; exit 1; }
make spectests > /dev/null || { echo 'make failed' ; exit 1; }
echo FINDING TESTS in $CLEAN/test
if [ $RETRY != 0 ]
then
  findtestsretry
else
  cd test
  findtests
fi
popd
pushd .
source util/setchplenv.bash > /dev/null
echo MAKING .
#make clean > /dev/null
make > /dev/null || { echo 'make failed' ; exit 1; }
make spectests > /dev/null || { echo 'make failed' ; exit 1; }
echo FINDING TESTS in ./test
if [ $RETRY != 0 ]
then
  findtestsretry
else
  cd test
  findtests
fi
popd

# Now, on each target node, rsync
echo RSYNCING the data to the remote nodes
cat $NODESFILE | parallel rsync -az --exclude ".svn/" --delete ./ {}:$DST
cat $NODESFILE | parallel rsync -az --exclude ".svn/" --delete $CLEAN/ {}:$CLEANDST

# Now, run the clean tests.
echo cat $CLEAN/test/.tests.txt x parallel --jobs $MAXJOBS --load $MAXLOAD --eta --sshloginfile $NODESFILE -L $TESTSPER "cd $CLEANDST; source util/setchplenv.bash > /dev/null; cd test; ../util/start_test -comm $COMM -logfile Logs/run-test-{#} -norecurse {} > /dev/null; grep '\[Error' Logs/run-test-{#}.summary" x $CLEAN/test/Logs/run-test.log
cat $CLEAN/test/.tests.txt | parallel --jobs $MAXJOBS --load $MAXLOAD --eta --sshloginfile $NODESFILE -L $TESTSPER "cd $CLEANDST; source util/setchplenv.bash > /dev/null; cd test; ../util/start_test -comm $COMM -logfile Logs/run-test-{#} -norecurse {} > /dev/null; grep '\[Error' Logs/run-test-{#}.summary" > $CLEAN/test/Logs/run-test.log
cat ./test/.tests.txt | parallel --jobs $MAXJOBS --load $MAXLOAD --eta --sshloginfile $NODESFILE -L $TESTSPER "cd $DST; source util/setchplenv.bash > /dev/null; cd test; ../util/start_test -comm $COMM -logfile Logs/run-test-{#} -norecurse {} > /dev/null; grep '\[Error' Logs/run-test-{#}.summary" >  test/Logs/run-test.log

sort $CLEAN/test/Logs/run-test.log > $CLEAN/test/Logs/run-test.log.sorted
sort ./test/Logs/run-test.log > ./test/Logs/run-test.log.sorted

grep -v '^Future' $CLEAN/test/Logs/run-test.log.sorted > $CLEAN/test/Logs/run-test.log.sorted.nofutures
grep -v '^Future' ./test/Logs/run-test.log.sorted > ./test/Logs/run-test.log.sorted.nofutures

comm -1 -3 $CLEAN/test/Logs/run-test.log.sorted.nofutures ./test/Logs/run-test.log.sorted.nofutures

