use CTypes;
use BlockDist;
use CommDiagnostics;

config param cArrSize = 3;
config const arrSize = 10;

const space = 1..arrSize;

const drDom = {space};
const blockDom = {space} dmapped new blockDist({space});

testBulkComm(drDom, drDom);
testBulkComm(drDom, blockDom);
testBulkComm(blockDom, drDom);
testBulkComm(blockDom, blockDom);

proc testBulkComm(srcDom, dstDom) {
  var Src: [srcDom] c_array(int, cArrSize);
  for src in Src {
    for param i in 0..#cArrSize {
      src[i] = i+1;
    }
  }

  var Dst: [dstDom] c_array(int, cArrSize);

  startCommDiagnostics();
  Dst = Src;
  stopCommDiagnostics();
  printCommDiagnosticsTable();

  writeln(Dst);
}
