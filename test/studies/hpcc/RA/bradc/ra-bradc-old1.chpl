// This version uses int cast sign comparison to get high bit

// BLC: Change cast to int into mask against high bit
// BLC: Implement a timer class and insert calls

const POLY:uint(64) = 0x0000000000000007;  // BLC: should be param, but causes error

config const totMemSize:uint(64) = 1000;
config const logTableSize = computeLogTableSize(totMemSize);
const tableSize = 1:uint(64) << logTableSize;

config const verify = true;
config const debug = false;

// BLC: note the prevalence of the n-1 upper bound motif
const tableDom = {0..tableSize:int-1};    // BLC: unfortunate cast
var Table: [tableDom] uint(64);

const numUpdates = 4*tableSize;
const updateDom = {0..numUpdates:int-1};  // BLC: unfortunate cast

config const numRandoms = 128;
config const errorTolerance = 0.01;


proc main() {
  writeln("Main table size = 2**", logTableSize, " = ", tableSize, " words");
  writeln("Number of updates = ", numUpdates);

  randomAccessUpdate();

  if (verify) then verifyResults();
}


// BLC: eliminate Ran array -- replace with per-thread local variable

proc randomAccessUpdate() {
  // BLC: might prefer the following line to be Table = tableDom;
  [i in tableDom] Table(i) = i:uint(64);      // BLC: unfortunate cast

  if debug then writeln("Table is: ", Table);

  // BLC: is it legal to make a config const/var local to a function?
  var ranDom = {0..numRandoms-1};
  // BLC: Would prefer this initialization to be:
  // var Ran: [ranDom] uint(64) = [i in ranDom] HPCCstarts((numUpdates/numRandoms) * i);
  var Ran: [ranDom] uint(64);
  // BLC: writing the following line results in the promotion-to-real problem
  // [i in ranDom] Ran(i) = HPCCstarts((numUpdates/numRandoms) * i);
  // BLC: resulting in the following unfortunate cast:
  // BLC: Getting the ambiguity declared is priority #1 for me 
  //      // I've made this mistake too many times now
  [i in ranDom] Ran(i) = HPCCstarts(((numUpdates:int(64)/numRandoms) * i));

  if debug then writeln("Ran is: ", Ran);

  for i in updateDom by numRandoms {
    // do serially to avoid errors in validation
    for j in ranDom {
      // BLC: This appears everywhere.  Might be nice to make it a macro-ish thing?
      Ran(j) = (Ran(j) << 1) ^ (if Ran(j):int(64) < 0 then POLY else 0:uint(64));
      Table((Ran(j) & (tableSize-1)):int) ^= Ran(j); // BLC: unfortunate cast
    }
  }

  if debug then writeln("Table = ", Table);
}


proc verifyResults() {

  var temp:uint(64) = 0x1;  // BLC: Can we rename this?
  for i in updateDom {
    temp = (temp << 1) ^ (if (temp:int(64) < 0) then POLY else 0:uint(64));
    Table((temp & (tableSize-1)):int) ^= temp;  // BLC: unforunate cast
  }

  var numErrors = 0;
  for i in tableDom {
    if (Table(i) != i:uint(64)) then
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
  if (n == 0) then return 0x1:uint(64);

  param m2DomSize = 64; // BLC: this is a magic number -- better name?
  var m2Dom = {0..m2DomSize-1};
  var m2: [m2Dom] uint(64);

  // BLC: isn't this redundantly computing the same array m2 over and
  // over again?
  var temp: uint(64) = 0x1;  // BLC: is there a better name for this?
  for i in m2Dom {
    m2(i) = temp;
    temp = (temp << 1) ^ (if temp:int(64) < 0 then POLY else 0:uint(64));
    temp = (temp << 1) ^ (if temp:int(64) < 0 then POLY else 0:uint(64));
  }

  var high = 62;    // BLC: magic number -- name?
  while ((n >> high) & 1) == 0 do
    high -= 1;

  var ran: uint(64) = 0x2;
  for i in 0..high-1 by -1 {
    var temp: uint(64) = 0;  // BLC: is there a better name for this?
    for j in m2Dom {
      if ((ran >> j:uint(64)) & 1) then temp ^= m2(j); // BLC: unfortunate cast
    }
    ran = temp;
    if ((n >> i) & 1) {
      ran = (ran << 1) ^ (if ran:int(64) < 0 then POLY else 0:uint(64));
    }
  }
  return ran;
}


// BLC: could replace this all by some sort of bpop + bit search 
// function?
proc computeLogTableSize(memsize) {
  param tableElemSize = 8;  // BLC: magic number == sizeof(uint(64))

  var elemsInTable = memsize / tableElemSize;
  var logTableSize: uint(64) = 0;

  while (elemsInTable > 1) {
    elemsInTable /= 2;
    logTableSize += 1;
  }

  return logTableSize;
}
