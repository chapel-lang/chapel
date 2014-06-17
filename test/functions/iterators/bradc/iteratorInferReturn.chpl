use Types;

type indexType = uint(64);
type tableElem = uint(64);


config const lgTableSize = 10;
const tableSize = 1 << lgTableSize;
const NUPDATE = 4*tableSize;


for i in RAStream(NUPDATE) {
  var j = i & (tableSize-1):indexType;
  writeln(j, " ", i);
  //  writeln(j:"%llu", i:"(%20llu)");
}


iter RAStream(numvals) {
  const POLY = 0x7:indexType;
  const hibit = 0x1:indexType << (numBits(indexType)-1);
  var val = 1:indexType;
  for i in 1..numvals {
    val = (val << 1) ^ (if (val & hibit) then POLY else 0);
    yield val;
  }
}
