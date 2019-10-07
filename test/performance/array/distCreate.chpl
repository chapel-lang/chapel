use Time;
use Memory;

use BlockDist;
use CyclicDist;
use BlockCycDist;


type elemType = int;

// assume homogeneity
const totMem = here.physicalMemory(unit = MemUnits.Bytes);
config const memFraction = 4;

config const correctness = false;
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
  if !correctness then t.start();
}

inline proc endDiag(name) {
  if !correctness {
    t.stop();
    writeln(name, ": ", t.elapsed());
    t.clear();
  }
}

inline proc endDiag(name, x) {
  if correctness {
    if x.size != nElems {
      halt(name , " has unexpected size");
    }
  }
  else {
    t.stop();
    writeln(name, ": ", t.elapsed());
    t.clear();
  }
}

const localDom = {1..nElems};

if !correctness && dist == distType.block {
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

if !correctness && dist == distType.cyclic {
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

if !correctness && dist == distType.blockCyc {
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
