config const n: int = 8;

if (n < 1 || n > 64) {
  halt("n must be within [1,64]");
}

type int64: int(64);

const tableSize: int = 2**n;
const TableInds: domain(1) dmapped(UMA) = [1..tableSize];
var T: [TableInds] int64 = 0;

const numUpdates: int = 2**(n+2);
const UpdateInds: domain(1) dmapped(Cyclic) = [1..numUpdates];

const numLoBits: int = 64-n;             -- e.g.,        10
const loBitMask: int64 = 2**numLowBits - 1;  -- e.g., 0x00003ff
const hiBitMask: int64 = ~loBitMask;         -- e.g., 0xffffc00

forall i in UpdateInds {
  var a_i: int64 = i**63 + i**2 + i + 1;
  var loc: int64 = (a_i & hiBitMask) >> numLoBits;
  T(loc) ^= a_i;
}

writeln("T is: ", T);
