#!/usr/bin/env bash
#SBATCH -o compile.out
#SBATCH -e compile.err
#SBATCH -t 47:55:00
#SBATCH -p risc5

echo $DATE
source util/setchplenv.bash

export MACHINE_TYPE="$(uname -m)"

export CHPL_TEST_PERF_DIR="$CHPL_HOME/test/perfdat/clbg_comparison"
export CHPL_TEST_TIMEOUT=600

start_test --performance --numtrials 10 test/studies/shootout/submitted
start_test --performance --numtrials 10 test/release/examples/benchmarks/shootout/chameneosredux.chpl
start_test --performance --numtrials 10 test/release/examples/benchmarks/shootout/chameneosredux-fast.chpl
start_test --performance --numtrials 10 test/release/examples/benchmarks/shootout/threadring.chpl
start_test --performance --numtrials 10 test/studies/shootout/thread-ring/lydia/thread-ring-coforall-begin.chpl
start_test --performance --numtrials 10 test/performance/elliot/no-op.chpl

if [ ${MACHINE_TYPE} == 'x86_64' ];
then
  # Set up some variables to use for testing with SMT and/or restricting to a single socket.
  # Assume 2 logical threads per core and 2 sockets since that's true of all the x86 machines we're testing with.
  export NUM_LOGICAL_THREADS=$(nproc --all)
  export NUM_CORES=$(($NUM_LOGICAL_THREADS / 2))
  export NUM_CORES_PER_SOCKET=$(($NUM_LOGICAL_THREADS / 4))
  export SOCKET_TASKSET_RANGE="0-$(($NUM_CORES_PER_SOCKET - 1)),$NUM_CORES-$(($NUM_CORES + $NUM_CORES_PER_SOCKET - 1))"

  # hyperthreads on all sockets
  export CHPL_TEST_PERF_DIR="$CHPL_HOME/test/perfdat/clbg_comparison_smt"
  export CHPL_RT_NUM_THREADS_PER_LOCALE=MAX_LOGICAL
  start_test --performance --numtrials 10 test/studies/shootout/submitted
  start_test --performance --numtrials 10 test/release/examples/benchmarks/shootout/chameneosredux.chpl
  start_test --performance --numtrials 10 test/release/examples/benchmarks/shootout/chameneosredux-fast.chpl
  start_test --performance --numtrials 10 test/release/examples/benchmarks/shootout/threadring.chpl
  start_test --performance --numtrials 10 test/studies/shootout/thread-ring/lydia/thread-ring-coforall-begin.chpl
  start_test --performance --numtrials 10 test/performance/elliot/no-op.chpl

  # no hyperthreads, single socket
  export CHPL_TEST_PERF_DIR="$CHPL_HOME/test/perfdat/clbg_comparison_single_socket"
  export CHPL_RT_NUM_THREADS_PER_LOCALE=$NUM_CORES_PER_SOCKET
  taskset -c $SOCKET_TASKSET_RANGE start_test --performance --numtrials 10 --execopts "-nl 1x1s" test/studies/shootout/submitted
  taskset -c $SOCKET_TASKSET_RANGE start_test --performance --numtrials 10 --execopts "-nl 1x1s" test/release/examples/benchmarks/shootout/chameneosredux.chpl
  taskset -c $SOCKET_TASKSET_RANGE start_test --performance --numtrials 10 --execopts "-nl 1x1s" test/release/examples/benchmarks/shootout/chameneosredux-fast.chpl
  taskset -c $SOCKET_TASKSET_RANGE start_test --performance --numtrials 10 --execopts "-nl 1x1s" test/release/examples/benchmarks/shootout/threadring.chpl
  taskset -c $SOCKET_TASKSET_RANGE start_test --performance --numtrials 10 --execopts "-nl 1x1s" test/studies/shootout/thread-ring/lydia/thread-ring-coforall-begin.chpl
  taskset -c $SOCKET_TASKSET_RANGE start_test --performance --numtrials 10 --execopts "-nl 1x1s" test/performance/elliot/no-op.chpl

  # hyperthreads on single socket
  export CHPL_TEST_PERF_DIR="$CHPL_HOME/test/perfdat/clbg_comparison_single_socket_smt"
  # NUM_CORES in this case is the same as the number of hyperthreads per socket.
  export CHPL_RT_NUM_THREADS_PER_LOCALE=$NUM_CORES
  taskset -c $SOCKET_TASKSET_RANGE start_test --performance --numtrials 10 --execopts "-nl 1x1s" test/studies/shootout/submitted
  taskset -c $SOCKET_TASKSET_RANGE start_test --performance --numtrials 10 --execopts "-nl 1x1s" test/release/examples/benchmarks/shootout/chameneosredux.chpl
  taskset -c $SOCKET_TASKSET_RANGE start_test --performance --numtrials 10 --execopts "-nl 1x1s" test/release/examples/benchmarks/shootout/chameneosredux-fast.chpl
  taskset -c $SOCKET_TASKSET_RANGE start_test --performance --numtrials 10 --execopts "-nl 1x1s" test/release/examples/benchmarks/shootout/threadring.chpl
  taskset -c $SOCKET_TASKSET_RANGE start_test --performance --numtrials 10 --execopts "-nl 1x1s" test/studies/shootout/thread-ring/lydia/thread-ring-coforall-begin.chpl
  taskset -c $SOCKET_TASKSET_RANGE start_test --performance --numtrials 10 --execopts "-nl 1x1s" test/performance/elliot/no-op.chpl
fi

# The only ARM system we're testing has two sockets, so just assume that here.
if [ ${MACHINE_TYPE} == 'aarch64' ];
then
  export NUM_CORES=$(nproc --all)
  export NUM_CORES_PER_SOCKET=$(($NUM_CORES / 2))
  export SOCKET_TASKSET_RANGE="0-$(($NUM_CORES_PER_SOCKET - 1))"
  export CHPL_TEST_PERF_DIR="$CHPL_HOME/test/perfdat/clbg_comparison_single_socket"
  export CHPL_RT_NUM_THREADS_PER_LOCALE=$NUM_CORES_PER_SOCKET
  taskset -c $SOCKET_TASKSET_RANGE start_test --performance --numtrials 10 --execopts "-nl 1x1s" test/studies/shootout/submitted
  taskset -c $SOCKET_TASKSET_RANGE start_test --performance --numtrials 10 --execopts "-nl 1x1s" test/release/examples/benchmarks/shootout/chameneosredux.chpl
  taskset -c $SOCKET_TASKSET_RANGE start_test --performance --numtrials 10 --execopts "-nl 1x1s" test/release/examples/benchmarks/shootout/chameneosredux-fast.chpl
  taskset -c $SOCKET_TASKSET_RANGE start_test --performance --numtrials 10 --execopts "-nl 1x1s" test/release/examples/benchmarks/shootout/threadring.chpl
  taskset -c $SOCKET_TASKSET_RANGE start_test --performance --numtrials 10 --execopts "-nl 1x1s" test/studies/shootout/thread-ring/lydia/thread-ring-coforall-begin.chpl
  taskset -c $SOCKET_TASKSET_RANGE start_test --performance --numtrials 10 --execopts "-nl 1x1s" test/performance/elliot/no-op.chpl
fi

