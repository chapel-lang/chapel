use Time;

type tableElem = uint(64);

config const totMemSizeInBytes = 1000;
const tableElemSize = numBits(tableElem)/bitsPerByte;
config const logTableSize = log2(totMemSizeInBytes / tableElemSize);
const tableSize = 0x1:tableElem << logTableSize;

config const verify = true;
config const debug = false;
config const printTiming = false;

// BLC: note the prevalence of the n-1 upper bound motif
const tableDom = {0..tableSize-1};
var Table: [tableDom] tableElem;

const numUpdates = 4*tableSize;
const updateDom = {0..numUpdates-1};

config const numRandoms = 128;
config const errorTolerance = 0.01;


proc main() {
  writeln("Main table size = 2**", logTableSize, " = ", tableSize, " words");
  writeln("Number of updates = ", numUpdates);

  var timer: Timer;
  timer.start();
  randomAccessUpdate();
  timer.stop();

  if (verify) then verifyResults();
  if (printTiming) then writeln("Elapsed time = ", timer.elapsed(), " seconds");
}


// BLC: eliminate Ran array -- replace with per-thread local variable
proc randomAccessUpdate() {
  Table = tableDom;

  if debug then writeln("Table is: ", Table);

  var ranDom = {0..numRandoms-1};
  var Ran: [ranDom] tableElem 
         = [i in ranDom] HPCCstarts((numUpdates:int(64)/numRandoms) * i);

  if debug then writeln("Ran is: ", Ran);

  for i in updateDom by numRandoms {
    // do serially to avoid errors in validation
    for j in ranDom {
      bitMunge(Ran(j));
      Table(Ran(j) & (tableSize-1)) ^= Ran(j);
    }
  }

  if debug then writeln("Table = ", Table);
}


proc verifyResults() {

  var temp: tableElem = 0x1;  // BLC: Can we rename this?
  for i in updateDom {
    bitMunge(temp);
    Table(temp & (tableSize-1)) ^= temp;
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


proc HPCCstarts(in n:int(64)) {
  param period = 1317624576693539401;

  while (n < 0) do
    n += period;
  while (n > period) do
    n -= period;
  if (n == 0) then return 0x1:tableElem;

  param m2DomSize = 64; // BLC: this is a magic number -- better name?
  var m2Dom = {0..m2DomSize-1};
  var m2: [m2Dom] tableElem;

  // BLC: isn't this redundantly computing the same array m2 over and
  // over again?
  var temp = 0x1: tableElem;  // BLC: is there a better name for this?
  for i in m2Dom {
    m2(i) = temp;
    bitMunge(temp);
    bitMunge(temp);
  }

  var high = 62;    // BLC: magic number -- name?
  while ((n >> high) & 1) == 0 do
    high -= 1;

  var ran = 0x2: tableElem;
  for i in 0..high-1 by -1 {
    var temp = 0:tableElem;  // BLC: is there a better name for this?
    for j in m2Dom {
      if ((ran >> j) & 1) then temp ^= m2(j);
    }
    ran = temp;
    if ((n >> i) & 1) {
      bitMunge(ran);
    }
  }
  return ran;
}


// BLC: would also like to see this fn inlined -- how to specify?
// BLC: better name for this fn?
proc bitMunge(inout x) {
  param POLY = 0x7:tableElem;
  param hibit = 0x1:tableElem << 63;
  x = (x << 1) ^ (if (x & hibit) then POLY else 0);
}
