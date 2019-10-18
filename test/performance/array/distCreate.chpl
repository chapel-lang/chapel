use Time;
use Memory;
use CommDiagnostics;

use BlockDist;
use CyclicDist;
use BlockCycDist;


type elemType = int;

// assume homogeneity
const totMem = here.physicalMemory(unit = MemUnits.Bytes);
config const memFraction = 4;

config const correctness = false;
config const commCount = false;
config const verboseComm = false;
config const verboseMem = false;

config const createArrays = true;

config const reportInit = true;
config const reportDeinit = true;

config const nElemsTiny = numLocales;
config const nElemsSmall = if correctness then 100 else 1000000;
config const nElemsLarge = numLocales*((totMem/numBytes(elemType))/memFraction);

enum arraySize { tiny, small, large};
enum distType { block, cyclic, blockCyc};

config const size = arraySize.tiny;
config const dist = distType.block;

const nElems = if size == arraySize.tiny then nElemsTiny else
               if size == arraySize.small then nElemsSmall else
               if size == arraySize.large then nElemsLarge else -1;


var t = new Timer();

inline proc shouldRunDiag(name) {
  if !reportInit && name.endsWith("Init") then
    return false;
  if !reportDeinit && name.endsWith("Deinit") then
    return false;

  return true;
}

inline proc startDiag(name) {
  if !shouldRunDiag(name) then return;

  if !correctness {
    if commCount {
      startCommDiagnostics();
    }
    else if verboseComm {
      startVerboseComm();
    }
    else if verboseMem {
      startVerboseMem();
    }
    else {
      t.start();
    }
  }
}

inline proc endDiag(name) {
  if !shouldRunDiag(name) then return;

  if !correctness {
    if commCount {
      stopCommDiagnostics();
      const d = getCommDiagnostics();
      writeln(name, "-GETS: ", + reduce (d.get + d.get_nb));
      writeln(name, "-PUTS: ", + reduce (d.put + d.put_nb));
      writeln(name, "-ONS: ", + reduce (d.execute_on + d.execute_on_fast +
                                        d.execute_on_nb));
      resetCommDiagnostics();
    }
    else if verboseComm {
      stopVerboseComm();
    }
    else if verboseMem {
      stopVerboseMem();
    }
    else {
      t.stop();
      writeln(name, ": ", t.elapsed());
      t.clear();
    }
  }
}

inline proc endDiag(name, x) {
  if correctness {
    if x.size != nElems {
      halt(name , " has unexpected size");
    }
  }
  endDiag(name);
}

const localDom = {1..nElems};

if correctness || dist == distType.block {
  { // weird blocks are necessary to measure deinit performance
    startDiag("domInit");
    const blockDom = localDom dmapped Block(boundingBox=localDom);
    endDiag("domInit", blockDom);
    if createArrays {
      {
        startDiag("arrInit");
        const blockArr: [blockDom] elemType;
        endDiag("arrInit", blockArr);

        startDiag("arrDeinit");
      }
      endDiag("arrDeinit");
    }
    startDiag("domDeinit");
  }
  endDiag("domDeinit");
}

if correctness || dist == distType.cyclic {
  { // weird blocks are necessary to measure deinit performance
    startDiag("domInit");
    const cyclicDom = localDom dmapped Cyclic(startIdx=localDom.first);
    endDiag("domInit", cyclicDom);
    if createArrays {
      {
        startDiag("arrInit");
        const cyclicArr: [cyclicDom] elemType;
        endDiag("arrInit", cyclicArr);

        startDiag("arrDeinit");
      }
      endDiag("arrDeinit");
    }
    startDiag("domDeinit");
  }
  endDiag("domDeinit");
}

if correctness || dist == distType.blockCyc {
  { // weird blocks are necessary to measure deinit performance
    startDiag("domInit");
    const blockCyclicDom = localDom dmapped BlockCyclic(startIdx=localDom.first,
                                                        blocksize=5);
    endDiag("domInit", blockCyclicDom);
    if createArrays {
      {
        startDiag("arrInit");
        const blockCyclicArr: [blockCyclicDom] elemType;
        endDiag("arrInit", blockCyclicArr);
        
        startDiag("arrDeinit");
      }
      endDiag("arrDeinit");
    }

    startDiag("domDeinit");
  }
  endDiag("domDeinit");
}
