use Time;
use CommDiagnostics;

enum op_t {
  opNone,
  opGet,
  opPut,
  opAMO,
  opFastOn,
  opOn
};

use op_t;

config param op = opNone;
config param opNeedsTask = (op == opOn);

const numWorkerNodes = numLocales - 1;
config const numTasksPerNode = min reduce ([i in 1..numWorkerNodes]
                                           Locales(i).maxTaskPar);

config const runSecs = 5.0;
config const minOpsPerTimerCheck = if opNeedsTask then 10**2 else 10**3;

config const printConfig = false;
config const printTimings = false;
config const printCommDiags = false;

var x0: int;
var x0Atomic: atomic int;

proc main() {
  if printConfig {
    writeln(numWorkerNodes, ' worker nodes, ',
            numTasksPerNode, ' worker tasks per node, ',
            'run for ', runSecs, ' seconds');
  }

  if printCommDiags {
    resetCommDiagnostics();
    startCommDiagnostics();
  }

  var numOpsPerNode: [1..numWorkerNodes] int;
  var timePerNode: [1..numWorkerNodes] real;

  var timer: Timer;
  if printTimings then
    timer.start();

  coforall locIdx in 1..numWorkerNodes with ( ref x0 ) {
    on Locales(locIdx) {
      var barTaskCnt: chpl__processorAtomicType(int); // for barrier()

      var numOpsPerTask: [1..numTasksPerNode] int;
      var timePerTask: [1..numTasksPerNode] real;

      coforall taskIdx in 1..numTasksPerNode with ( ref x0 ) {
        var nopsAtCheck = minOpsPerTimerCheck;
        var nops: int;

        var t: Timer;
        var tElapsed: real;

        // We want all nodes & tasks communicating at once.
        barrier(barTaskCnt);

        t.start();

        while true {
          //
          // We do comms until runSecs has passed in this task, using
          // nopsAtCheck to limit how much overhead we spend checking
          // for that.
          //
          if nops == nopsAtCheck {
            tElapsed = t.elapsed();
            if tElapsed >= runSecs then break;
            nopsAtCheck = (nops * (0.75 * runSecs / tElapsed)):int;
            if nopsAtCheck - nops < minOpsPerTimerCheck then
              nopsAtCheck = nops + minOpsPerTimerCheck;
          }
          doOneOp(nops);
          nops += 1;
        }

        numOpsPerTask(taskIdx) = nops;
        timePerTask(taskIdx) = tElapsed;
      }

      numOpsPerNode(locIdx) = + reduce numOpsPerTask;
      timePerNode(locIdx) = + reduce timePerTask;
    }
  }

  if printTimings {
    const numOpsTotal = + reduce numOpsPerNode;
    const timeTotal = + reduce timePerNode;
    const timeAvg = timeTotal / (numTasksPerNode * numWorkerNodes);
    const mOpsPerSecAvg = (numOpsTotal / timeAvg) * 1e-6;
    const mOpsPerSecPerTask = (numOpsTotal / timeTotal) * 1e-6;

    writeln('numOps = ', numOpsTotal);
    writeln('Execution time = ', timeAvg);
    writeln('Performance (mOps/sec) = ', mOpsPerSecAvg);
    writeln('Performance (mOps/sec/Task) = ', mOpsPerSecPerTask);
  }

  if printCommDiags {
    stopCommDiagnostics();
    writeln(getCommDiagnostics());
  }
}


inline proc doOneOp(nops) {
  if op == opNone {
  }
  else if op == opGet {
    if nops == 0 then x0 = 0; // prevent compiler loop-hoisting GET
    infiniteSink(x0);
  }
  else if op == opPut {
    x0 = infiniteSource();
  }
  else if op == opAMO {
    x0Atomic.xor(1);
  }
  else if op == opFastOn {
    on Locales(0) do ;
  }
  else if op == opOn {
    on Locales(0) do emptyFn();
  }
}


//
// This is a simple nonreusable barrier.  If desired, we could replace
// it with something better when the Barriers module is improved.
//
var barNodeCnt: atomic int;

proc barrier(barTaskCnt) {
  if barTaskCnt.fetchAdd(1) == 0 {
    barNodeCnt.add(1);
    barNodeCnt.waitFor(numWorkerNodes);
  }
  barTaskCnt.waitFor(numTasksPerNode);
}


extern proc emptyFn();
extern proc infiniteSink(x: int);
extern proc infiniteSource(): int;
