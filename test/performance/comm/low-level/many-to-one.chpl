use Time;
use CommDiagnostics;
use AllLocalesBarriers;
use BlockDist;

enum op_t {
  opNone,
  opGet,
  opUnorderedGet,
  opPut,
  opFetchAMO,
  opAMO,
  opUnorderedAMO,
  opFastOn,
  opOn
};

use op_t;

config param op = opNone;
config param opNeedsTask = (op == opOn);

const numWorkerNodes = numLocales - 2;
config const numTasksPerNode = min reduce ([i in 0..#numLocales]
                                           Locales(i).maxTaskPar);

config const runSecs = 5.0;
config const minOpsPerTimerCheck = if opNeedsTask then 10**2 else 10**3;

config const printConfig = false;
config const doWarmupPass = true;
config const printWarmupTiming = false;
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
            numTasksPerNode, ' worker tasks per node, ',
            'run for ', runSecs, ' seconds');
  }

  if doWarmupPass {
    //
    // Warm up.  This avoids including overheads associated with the
    // first communication transactions in the measured performance.
    // It's particularly necessary for comm=ofi with providers that
    // connect endpoints dynamically upon first communication, for
    // example.
    //
    var tWarmup: Timer;
    tWarmup.start();

    coforall locIdx in 0..#numLocales {
      on Locales(locIdx) {
        coforall taskIdx in 1..numTasksPerNode {
          if locIdx > 1 {
            on Locales(1) do emptyFn();
          }
        }
      }
    }

    if printWarmupTiming {
      writeln('Warmup time: ', tWarmup.elapsed(), 's');
    }
  }

  //
  // Now do the measured operations.
  //
  if printCommDiags {
    resetCommDiagnostics();
    startCommDiagnostics();
  }

  var numOpsOnNodes: [0..#numLocales] int;
  var timeOnNodes: [0..#numLocales] real;

  allLocalesBarrier.reset(numTasksPerNode);

  coforall locIdx in 0..#numLocales {
    on Locales(locIdx) {
      if locIdx == 0 || locIdx == 1 {
        //
        // Locales 0 and 1 must participate in the barrier but otherwise
        // are not involved.  Locale 0 is waiting for the coforall-stmt
        // to complete, and locale 1 is the many-to-one target node.
        //
        coforall 1..numTasksPerNode do allLocalesBarrier.barrier() ;
      } else {
        //
        // Locales 2..numLocales-1 generate the many-to-one ops, which
        // target locale 1.
        //
        ref x = remoteVar(1);
        ref xAtomic = remoteVarAtomic(1);

        var numOpsOnTasks: [1..numTasksPerNode] int;
        var timeOnTasks: [1..numTasksPerNode] real;

        coforall taskIdx in 1..numTasksPerNode with (ref x, ref xAtomic) {
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
            doOneOp(nops, x, xAtomic);
            nops += 1;
          }

          numOpsOnTasks(taskIdx) = nops;
          timeOnTasks(taskIdx) = tElapsed;
        }

        numOpsOnNodes(locIdx) = + reduce numOpsOnTasks;
        timeOnNodes(locIdx) = + reduce timeOnTasks;
      }
    }
  }

  if printTimings {
    const numOpsTotal = + reduce numOpsOnNodes;
    const timeTotal = + reduce timeOnNodes;
    const opRatePerSource = (numOpsTotal / timeTotal) / 1e6;
    const numSources = numTasksPerNode * numWorkerNodes;
    const opRateToTarget = opRatePerSource * numSources;

    writeln('numOps = ', numOpsTotal);
    writeln('Execution time = ', timeTotal / numSources);
    writeln('Performance (mOps/sec/source) = ', opRatePerSource);
    writeln('Performance (mOps/sec) = ', opRateToTarget);
  }

  if printCommDiags {
    stopCommDiagnostics();
    writeln(getCommDiagnostics());
  }
}


inline proc doOneOp(nops, ref x, ref xAtomic) {
  if op == opNone {
  }
  else if op == opGet {
    if nops == 0 then x = 0; // prevent compiler loop-hoisting GET
    infiniteSink(x);
  }
  else if op == opUnorderedGet {
    if nops == 0 then x = 0; // prevent compiler loop-hoisting GET
    var xCopy: x.type;
    use UnorderedCopy;
    unorderedCopy(xCopy, x);
    infiniteSink(xCopy);
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
  else if op == opUnorderedAMO {
    use UnorderedAtomics;
    xAtomic.unorderedAdd(1);
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
