const POLY:uint = 0x0000000000000007u;  // BLC: should be param?
//      PERIOD = 1317624576693539401;

config const verify = false;

config const totMemSize = 1000u;
config const logTableSize = computeLogTableSize(totMemSize);
const tableSize = 1 << logTableSize;

config const errorTolerance = 0.01;

const tableDom = [0..tableSize:int-1];    // BLC: unfortunate cast
var Table: [tableDom] uint;

const numUpdates = 4*tableSize;
const updateDom = [0..numUpdates:int-1];  // BLC: unfortunate cast

//var NUPDATE: int;

def main() {
  writeln("Main table size = 2**", logTableSize, " = ", tableSize, " words");
  writeln("Number of updates = ", numUpdates);

  //  RandomAccessUpdate();

  if (verify) {
    var numErrors = VerifyResults();

    writeln("Found ", numErrors, "errors in ", tableSize, " locations");
    if (numErrors >= tableSize*errorTolerance) {
      writeln("(passed)");
    } else {
      writeln("(failed)");
    }
  }
}


def VerifyResults() {
  var temp = 0x1u;
  for i in updateDom {
    temp = (temp << 1) ^ (if (temp < 0) then POLY else 0);
    Table(temp & (tableSize-1)) ^= temp;
  }

  var numErrors = 0;
  for i in tableDom {
    if (Table(i) != i) then
      numErrors += 1;
  }

  return numErrors;
}


def computeLogTableSize(memsize: uint) {
  param tableElemSize = 8;
  param fractionOfMemory = 0.5;

  var elemsInTable = (memsize * fractionOfMemory):uint / tableElemSize;
  var logTableSize = 0u;

  while (elemsInTable > 1) {
    elemsInTable /= 2;
    logTableSize += 1;
  }

  return logTableSize;
}
