use Time;
use CommDiagnostics;

const numWorkerNodes = numLocales - 1;
config const numTasksPerNode = min reduce ([i in 1..numWorkerNodes]
                                           Locales(i).maxTaskPar);

config const numOps = 10**6;
const numOpsPerTask = numOps / (numWorkerNodes * numTasksPerNode);

config const printConfig = false;
config const printTimings = false;
config const printCommDiags = false;

var x0: int;
var x0Atomic: atomic int;

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

proc main() {
  if printConfig {
    writeln(numWorkerNodes, ' worker nodes, ',
            numTasksPerNode, ' worker tasks per node, ',
            numOps, ' ', op, (if numOps == 1 then '' else 's'));
  }

  if printCommDiags {
    resetCommDiagnostics();
    startCommDiagnostics();
  }

  var timer: Timer;

  timer.start();

  coforall locIdx in 1..numWorkerNodes with ( ref x0 ) {
    on Locales(locIdx) {
      coforall 1..numTasksPerNode with ( ref x0 ) {
        for i in 1..numOpsPerTask {
          select op {
            when opNone {
            }

            when opGet {
              if i == 1 then x0 = 0; // prevent compiler loop-hoisting GET
              infiniteSink(x0);
            }

            when opPut {
              x0 = infiniteSource();
            }

            when opAMO {
              x0Atomic.xor(1);
            }

            when opFastOn {
              on Locales(0) do ;
            }

            when opOn {
              on Locales(0) do emptyFn();
            }
          }
        }
      }
    }
  }

  if printTimings {
    timer.stop();
    const mOpsPerSecPerTask = (numOpsPerTask / timer.elapsed()) * 1e-6;
    const mOpsPerSec = mOpsPerSecPerTask * numTasksPerNode * numWorkerNodes;

    writeln('Execution time = ', timer.elapsed());
    writeln('Performance (mOps/sec) = ', mOpsPerSec);
    writeln('Performance (mOps/sec/Task) = ', mOpsPerSecPerTask);
  }

  if printCommDiags {
    stopCommDiagnostics();
    writeln(getCommDiagnostics());
  }
}


extern proc emptyFn();
extern proc infiniteSink(x: int);
extern proc infiniteSource(): int;
