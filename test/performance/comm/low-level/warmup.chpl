use Time;
use CommDiagnostics;
use BlockDist;

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

const numWorkerNodes = numLocales - 2;
config const numTasksPerNode = min reduce ([i in 0..#numLocales]
                                           Locales(i).maxTaskPar);

config const printConfig = false;
config const printTimings = false;
config const printCommDiags = false;

const remoteVarSpace = LocaleSpace dmapped Block(LocaleSpace);
var remoteVar: [remoteVarSpace] int;
var remoteVarAtomic: [remoteVarSpace] atomic int;

proc main() {
  if numWorkerNodes < 1 then
    halt('This program needs at least 3 nodes.');

  if printConfig {
    writeln(numWorkerNodes, ' worker nodes, ',
            numTasksPerNode, ' worker tasks per node');
  }

  if printCommDiags {
    resetCommDiagnostics();
    startCommDiagnostics();
  }

  var t: Timer;
  t.start();

  coforall locIdx in 0..#numLocales {
    on Locales(locIdx) {
      if locIdx > 1 {
        //
        // Locales 2..numLocales-1 initiate the many-to-one ops, which
        // target locale 1.
        //
        ref x = remoteVar(1);
        ref xAtomic = remoteVarAtomic(1);

        coforall taskIdx in 1..numTasksPerNode with (ref x, ref xAtomic) {
          doOneOp(x, xAtomic);
        }
      }
    }
  }

  if printTimings {
    writeln('Warmup time = ', t.elapsed(), 's');
  }

  if printCommDiags {
    stopCommDiagnostics();
    writeln(getCommDiagnostics());
  }
}


inline proc doOneOp(ref x, ref xAtomic) {
  if op == opNone {
  }
  else if op == opGet {
    infiniteSink(x);
  }
  else if op == opPut {
    x = infiniteSource();
  }
  else if op == opFetchAMO {
    infiniteSink(xAtomic.fetchAdd(1));
  }
  else if op == opAMO {
    xAtomic.add(1);
  }
  else if op == opFastOn {
    on Locales(1) do ;
  }
  else if op == opOn {
    on Locales(1) do emptyFn();
  }
}

extern proc emptyFn();
extern proc infiniteSink(x: int);
extern proc infiniteSource(): int;
