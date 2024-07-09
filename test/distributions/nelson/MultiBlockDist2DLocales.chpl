
use BlockDist;

config const size : int = 8;
const sizeRange = 0..#(size : int(64));


proc printBlockLocaleAssignments(locs) {
  const myBlockDist = new blockDist(rank=2, idxType=int(64),
                                                targetLocales=locs,
                                                boundingBox={sizeRange,sizeRange});
  const blockDom : domain(2, int(64)) dmapped myBlockDist = {sizeRange, sizeRange};

  var array : [blockDom] int;

  for ij in array.domain do
    array(ij) = array(ij).locale.id;

  writeln(array);
}


proc main() {

  writeln("Number of locales = ", numLocales);

  writeln( "Block by row:" );
  printBlockLocaleAssignments( reshape(Locales, {1..numLocales, 1..1}) );

  writeln( "Block by column:" );
  printBlockLocaleAssignments( reshape(Locales, {1..1, 1..numLocales}) );

  writeln( "Block following shape as 6-element 2D array of locales:" );
  const rowcount = 2, colcount = 3;
  const D : domain(2) = {0..#rowcount, 0..#colcount};
  const L : [D] locale = [(i,j) in D] Locales( (i * colcount + j) % numLocales);
  printBlockLocaleAssignments(L);

  writeln("done.");
}
  
