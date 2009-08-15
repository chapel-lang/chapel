
use MultiBlockDist;

config const size : int = 8;
const sizeRange = 0..#(size : int(64));


const blockDist = new Block(rank=1,
                            bbox=[sizeRange]);
const blockDom : domain(1, int(64)) distributed blockDist = [sizeRange];

var array : [i in blockDom] int(64) = i;


forall i in array {
  for j in 1..i {
    write(j," ");
    if (j == 5) then {
      write("...");
      break; // this used to cause an error; not anymore.
    }
  }
  writeln(".");
}

