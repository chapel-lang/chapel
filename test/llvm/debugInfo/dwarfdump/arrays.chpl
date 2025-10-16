use Debugger;
use BlockDist;
use CyclicDist;

proc main() {

  // DWARFDUMP: myArr1D
  // DWARFDUMP: [domain(1,int(64),one)] int(64)
  // DWARFDUMP: BaseRectangularArr(1,int(64),one,int(64))
  // DWARFDUMP: BaseArrOverRectangularDom(1,int(64),one)
  // DWARFDUMP: chpl_ddataResizePolicy
  // DWARFDUMP: BaseArr
  var myArr1D: [1..10] int;
  // DWARFDUMP: myArr2D
  // DWARFDUMP: [domain(2,int(64),one)] real(32)
  // DWARFDUMP: BaseRectangularArr(2,int(64),one,real(32))
  // DWARFDUMP: BaseArrOverRectangularDom(2,int(64),one)
  const myArr2D: [1..10, 1..10] real(32);

  // DWARFDUMP: myArr2DBlock
  // DWARFDUMP: [BlockDom(2,int(64),one,unmanaged DefaultDist)] int(64)
  var myArr2DBlock = blockDist.createArray({1..10, 1..10}, int);
  // DWARFDUMP: myArr1DCyclic
  // DWARFDUMP: [CyclicDom(1,int(64),one)] uint(8)
  var myArr1DCyclic = cyclicDist.createArray({1..10}, uint(8));

  writeln(myArr1D, myArr2D, myArr2DBlock, myArr1DCyclic, sep=" | ");
  breakpoint;
}
