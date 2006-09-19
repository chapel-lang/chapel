const POLY = 0x0000000000000007u;  // BLC: should be param, but causes error
//      PERIOD = 1317624576693539401;

config const totMemSize = 1000u;
config const logTableSize = computeLogTableSize(totMemSize);
const tableSize = 1 << logTableSize;

config const errorTolerance = 0.01;

config const verify = true;
config const debug = false;

const tableDom = [0..tableSize:int-1];    // BLC: unfortunate cast
var Table: [tableDom] uint;

const numUpdates = 4*tableSize;
const updateDom = [0..numUpdates:int-1];  // BLC: unfortunate cast


def main() {
  writeln("Main table size = 2**", logTableSize, " = ", tableSize, " words");
  writeln("Number of updates = ", numUpdates);

  randomAccessUpdate();

  if (verify) then verifyResults();
}


def randomAccessUpdate() {
  Table = tableDom:uint;
  writeln("Table is: ", Table);
}


def verifyResults() {
  var temp = 0x1u;
  for i in updateDom {
    temp = (temp << 1) ^ (if (temp < 0) then POLY else 0u);
    Table((temp & (tableSize-1)):int) ^= temp;  // BLC: unforunate cast
  }

  var numErrors = 0;
  for i in tableDom {
    if (Table(i) != i) then
      numErrors += 1;
  }

  write("Found ", numErrors, " errors in ", tableSize, " locations ");
  if (numErrors <= errorTolerance*tableSize) {
    writeln("(passed)");
  } else {
    writeln("(failed)");
  }
}


def computeLogTableSize(memsize) {
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
