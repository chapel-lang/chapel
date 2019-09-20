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
config const nElemsSmall = if correctness then 100 else 1000000;
config const nElemsLarge = numLocales*((totMem/numBytes(elemType))/memFraction);

config const small = true;
const nElems = if small then nElemsSmall else nElemsLarge;

var t = new Timer();

inline proc startDiag() {
  if !correctness then t.start();
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
  endDiag("blockDom", blockDom);

  startDiag();
  const blockArr: [blockDom] elemType;
  endDiag("blockArr", blockArr);
}

{
  startDiag();
  const cyclicDom = localDom dmapped Cyclic(startIdx=localDom.first);
  endDiag("cyclicDom", cyclicDom);

  startDiag();
  const cyclicArr: [cyclicDom] elemType;
  endDiag("cyclicArr", cyclicArr);
}

{
  startDiag();
  const blockCyclicDom = localDom dmapped BlockCyclic(startIdx=localDom.first,
                                                      blocksize=5);
  endDiag("blockCyclicDom", blockCyclicDom);

  startDiag();
  const blockCyclicArr: [blockCyclicDom] elemType;
  endDiag("blockCyclicArr", blockCyclicArr);
}
