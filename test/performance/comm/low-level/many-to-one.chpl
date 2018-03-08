use Time;
use CommDiagnostics;
use AllLocalesBarriers;

enum op_t {
  opNone,
  opGet,
  opPut,
  opFetchAMO,
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

  allLocalesBarrier.reset(numTasksPerNode);

  coforall locIdx in 0..#numLocales {
    if locIdx == 0 {
      //
      // Locale 0 must participate in the barrier but otherwise is not
      // involved, since it is the many-to-one target node.
      //
      coforall 1..numTasksPerNode do allLocalesBarrier.barrier() ;
    } else {
      //
      // Locales 1..numLocales-1 generate the many-to-one ops.
      //
      on Locales(locIdx) {
        var numOpsPerTask: [1..numTasksPerNode] int;
        var timePerTask: [1..numTasksPerNode] real;

        coforall taskIdx in 1..numTasksPerNode {
          var nopsAtCheck = minOpsPerTimerCheck;
          var nops: int;

          var t: Timer;
          var tElapsed: real;

          allLocalesBarrier.barrier();

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
  else if op == opFetchAMO {
    infiniteSink(x0Atomic.fetchAdd(1));
  }
  else if op == opAMO {
    x0Atomic.add(1);
  }
  else if op == opFastOn {
    on Locales(0) do ;
  }
  else if op == opOn {
    on Locales(0) do emptyFn();
  }
}

extern proc emptyFn();
extern proc infiniteSink(x: int);
extern proc infiniteSource(): int;
