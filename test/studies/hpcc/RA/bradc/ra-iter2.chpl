use Types;

type indexType = uint(64);
type tableElem = uint(64);


config const lgTableSize = 10;
const tableSize = 1 << lgTableSize;
const NUPDATE = 4*tableSize;


for i in RAStream(NUPDATE/4) {
  var j = i & (tableSize-1):indexType;
  writeln(j, " ", i);
  //  writeln(j:"%llu", i:"(%20llu)");
}
writeln("----");
for i in RAStream(NUPDATE/4, NUPDATE/4) {
  var j = i & (tableSize-1):indexType;
  writeln(j, " ", i);
  //  writeln(j:"%llu", i:"(%20llu)");
}
writeln("----");
for i in RAStream(NUPDATE/4, 2*NUPDATE/4) {
  var j = i & (tableSize-1):indexType;
  writeln(j, " ", i);
  //  writeln(j:"%llu", i:"(%20llu)");
}
writeln("----");
for i in RAStream(NUPDATE/4, 3*NUPDATE/4) {
  var j = i & (tableSize-1):indexType;
  writeln(j, " ", i);
  //  writeln(j:"%llu", i:"(%20llu)");
}


iter RAStream(numvals, start=0): indexType {
  const POLY = 0x7:indexType;
  const hibit = 0x1:indexType << (numBits(indexType)-1);
  var val = getNthRandom(start);
  for i in 1..numvals {
    val = (val << 1) ^ (if (val & hibit) then POLY else 0);
    yield val;
  }
}


proc getNthRandom(in n:int(64)) {
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
