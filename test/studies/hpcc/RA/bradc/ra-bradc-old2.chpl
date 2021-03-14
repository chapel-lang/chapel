// BLC: Implement a timer class and insert calls

config const totMemSize = 1000;
param tableElemSize = 8;  // BLC: magic number == sizeof(uint(64))
config const logTableSize = log2(totMemSize / tableElemSize);
const tableSize = 0x1:uint(64) << logTableSize:uint; // BLC: unfortunate cast

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
  [i in tableDom] Table(i) = i:uint(64);  // BLC: unfortunate cast

  if debug then writeln("Table is: ", Table);

  var ranDom = {0..numRandoms-1};
  // BLC: Would prefer this initialization to be:
  // var Ran: [ranDom] uint(64) = [i in ranDom] HPCCstarts((numUpdates/numRandoms) * i);
  var Ran: [ranDom] uint(64);
  // BLC: writing the following line results in the promotion-to-real problem
  // [i in ranDom] Ran(i) = HPCCstarts((numUpdates/numRandoms) * i);
  // BLC: resulting in the following unfortunate cast:
  // BLC: Getting the ambiguity declared is priority #1 for me 
  //      // I've made this mistake too many times now
  [i in ranDom] Ran(i) = HPCCstarts(((numUpdates:int/numRandoms) * i));

  if debug then writeln("Ran is: ", Ran);

  for i in updateDom by numRandoms {
    // do serially to avoid errors in validation
    for j in ranDom {
      Ran(j) = bitMunge(Ran(j));
      Table((Ran(j) & (tableSize-1)):int) ^= Ran(j); // BLC: unfortunate cast
    }
  }

  if debug then writeln("Table = ", Table);
}


proc verifyResults() {

  var temp= 0x1:uint(64);  // BLC: Can we rename this?
  for i in updateDom {
    temp = bitMunge(temp);
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


proc HPCCstarts(in n:int(64)) { // BLC: unfortunate type specification
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
  var temp = 0x1: uint(64);  // BLC: is there a better name for this?
  for i in m2Dom {
    m2(i) = temp;
    temp = bitMunge(temp);
    temp = bitMunge(temp);
  }

  var high = 62;    // BLC: magic number -- name?
  while ((n >> high) & 1) == 0 do
    high -= 1;

  var ran = 0x2: uint(64);
  for i in 0..high-1 by -1 {
    var temp = 0:uint(64);  // BLC: is there a better name for this?
    for j in m2Dom {
      if ((ran >> j:uint) & 1) then temp ^= m2(j); // BLC: unfortunate cast
    }
    ran = temp;
    if ((n >> i) & 1) {
      ran = bitMunge(ran);
    }
  }
  return ran;
}


// BLC: would like x to be an inout
// BLC: would also like to see this fn inlined -- how to specify?
// BLC: better name for this fn?
proc bitMunge(x) {
  const POLY: uint(64) = 0x0000000000000007;  // BLC: should be param, but causes error
  param hibit: uint(64) = 0x8000000000000000; // BLC: would like to write this: 0x1u << 63, but doesn't work
  return (x << 1) ^ (if (x & hibit) then POLY else 0:uint(64));  // BLC: cast is unfortunate
}
