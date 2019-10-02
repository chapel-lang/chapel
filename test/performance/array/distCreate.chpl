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

config const size = arraySize.tiny;

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

{
  startDiag();
  const blockDom = localDom dmapped Block(boundingBox=localDom);
  endDiag("blockDomInit", blockDom);
  {
    startDiag();
    const blockArr: [blockDom] elemType;
    endDiag("blockArrInit", blockArr);

    startDiag();
  }
  endDiag("blockArrDeinit");

  startDiag();
}
endDiag("blockDomDeinit");

{
  startDiag();
  const cyclicDom = localDom dmapped Cyclic(startIdx=localDom.first);
  endDiag("cyclicDomInit", cyclicDom);
  {

    startDiag();
    const cyclicArr: [cyclicDom] elemType;
    endDiag("cyclicArrInit", cyclicArr);

    startDiag();
  }
  endDiag("cyclicArrDeinit");

  startDiag();
}
endDiag("cyclicDomDeinit");

{
  startDiag();
  const blockCyclicDom = localDom dmapped BlockCyclic(startIdx=localDom.first,
                                                      blocksize=5);
  endDiag("blockCyclicDomInit", blockCyclicDom);
  {

    startDiag();
    const blockCyclicArr: [blockCyclicDom] elemType;
    endDiag("blockCyclicArrInit", blockCyclicArr);
    
    startDiag();
  }
  endDiag("blockCyclicArrDeinit");

  startDiag();
}
endDiag("blockCyclicDomDeinit");
