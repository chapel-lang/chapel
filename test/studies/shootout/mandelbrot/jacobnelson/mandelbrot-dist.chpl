
use BlockDist;


// for binary output
extern proc putchar(i : int) : int;

config const size: int(64) = 200;
config const iters = 50;
  
const sizeRange = 0..#size;
const iterRange = 0..#iters;

var Dist = new dmap (new Block(rank=2, idxType=int(64), boundingBox={sizeRange, sizeRange}));
var Dom: domain(2, int(64)) dmapped Dist = {sizeRange, sizeRange};
var Bitmap: [Dom] bool;


forall(i,j) in Dom do {
  Bitmap(i,j) = work(i,j);
 }


proc work(x, y) : bool {
  const fy: complex = 2i * y / size - 1i;
  const c: complex = 2.0 * x / size - 1.5 + fy;
  var z : complex = 0i;
  
  for i in iterRange do {
    z = z * z + c;
    if abs(z) >= 2.0 then {
      return true;
    }
  }
  return false;
}


const byteRange = 0..#(size/8);

var ByteDist = new dmap(new Block(rank=2, idxType=int(64), boundingBox={byteRange, sizeRange}));
var ByteDom: domain(2, int(64)) dmapped ByteDist = {byteRange, sizeRange};
var OutputArray: [ByteDom] uint(8);

for (i,j) in ByteDom do {
  for k in 0..7 do {
    OutputArray(i,j) <<= 1;
    OutputArray(i,j) |= (!Bitmap((i<<3)+k,j)):uint(8);
  }
 }



writeln("P4");
writeln(size, " ", size);

for y in sizeRange do {
  for x in byteRange do {
    putchar(OutputArray(x,y));
  }
 }



