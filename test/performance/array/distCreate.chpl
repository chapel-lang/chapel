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

inline proc startDiag() {
  if !correctness {
    if commCount {
      startCommDiagnostics();
    }
    else {
      t.start();
    }
  }
}

inline proc endDiag(name) {
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
    startDiag();
    const blockDom = localDom dmapped Block(boundingBox=localDom);
    endDiag("domInit", blockDom);
    {
      startDiag();
      const blockArr: [blockDom] elemType;
      endDiag("arrInit", blockArr);

      startDiag();
    }
    endDiag("arrDeinit");

    startDiag();
  }
  endDiag("domDeinit");
}

if correctness || dist == distType.cyclic {
  { // weird blocks are necessary to measure deinit performance
    startDiag();
    const cyclicDom = localDom dmapped Cyclic(startIdx=localDom.first);
    endDiag("domInit", cyclicDom);
    {

      startDiag();
      const cyclicArr: [cyclicDom] elemType;
      endDiag("arrInit", cyclicArr);

      startDiag();
    }
    endDiag("arrDeinit");

    startDiag();
  }
  endDiag("domDeinit");
}

if correctness || dist == distType.blockCyc {
  { // weird blocks are necessary to measure deinit performance
    startDiag();
    const blockCyclicDom = localDom dmapped BlockCyclic(startIdx=localDom.first,
                                                        blocksize=5);
    endDiag("domInit", blockCyclicDom);
    {

      startDiag();
      const blockCyclicArr: [blockCyclicDom] elemType;
      endDiag("arrInit", blockCyclicArr);
      
      startDiag();
    }
    endDiag("arrDeinit");

    startDiag();
  }
  endDiag("domDeinit");
}
