param POLY = 0x0000000000000007:uint(64);
//      PERIOD = 1317624576693539401;

config const verify = false;

config const totMemSize = 1000:uint;
config const logTableSize = computeLogTableSize(totMemSize);
const tableSize = 1 << logTableSize;

config const errorTolerance = 0.01;

const tableDom = [0..tableSize-1];
var Table: [tableDom] uint(64);

const numUpdates = 4*tableSize;
const updateDom = [0..numUpdates-1];

//var NUPDATE: int;

proc main() {
  writeln("Main table size = 2**", logTableSize, " = ", tableSize, " words");
  writeln("Number of updates = ", numUpdates);

  RandomAccessUpdate();

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


proc VerifyResults() {
  var temp = 0x1:uint(64);
  for i in updateDom {
    temp = (temp << 1) ^ (if (temp < 0) then POLY else 0);
    Table(temp & (tableSize-1)) ^= temp;
  }

  var numError = 0;
  for i in tableDom {
    if (Table(i) != i) then
      numErrors += 1;
  }

  return numErrors;
}


proc computeLogTableSize(memsize: uint) {
  param tableElemSize = 8;
  param fractionOfMemory = 0.5;

  var elemsInTable = (memsize * fractionOfMemory):uint / tableElemSize;
  var logTableSize = 0:uint;

  while (elemsInTable > 1) {
    elemsInTable /= 2;
    logTableSize += 1;
  }

  return logTableSize;
}
