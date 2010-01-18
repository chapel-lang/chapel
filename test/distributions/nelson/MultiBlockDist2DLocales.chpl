
use BlockDist;

config const size : int = 8;
const sizeRange = 0..#(size : int(64));


def printBlockLocaleAssignments(locs) {
  const blockDist = new dist(new Block(rank=2, idxType=int(64),
                                                targetLocales=locs,
                                                bbox=[sizeRange, sizeRange]));
  const blockDom : domain(2, int(64)) distributed blockDist = [sizeRange, sizeRange];

  var array : [blockDom] int;

  for ij in array.domain do
    array(ij) = array(ij).locale.id;

  writeln(array);
}


def main() {

  writeln("Number of locales = ", numLocales);

  writeln( "Block by row:" );
  printBlockLocaleAssignments( reshape(Locales, [1..numLocales, 1..1]) );

  writeln( "Block by column:" );
  printBlockLocaleAssignments( reshape(Locales, [1..1, 1..numLocales]) );

  writeln( "Block following shape as 6-element 2D array of locales:" );
  const rowcount = 2, colcount = 3;
  const D : domain(2) = [0..#rowcount, 0..#colcount];
  const L : [(i,j) in D] locale = Locales( (i * colcount + j) % numLocales);
  printBlockLocaleAssignments(L);

  writeln( "Emulate cyclic distribution by repeating locales in 1D array:" );
  // warning: creates one task per locale assignment; may create too many threads
  const oversubscribed : int = size*size;
  const cyclicLocales : [loc in 0..#oversubscribed] locale = Locales(loc % numLocales);
  printBlockLocaleAssignments(cyclicLocales); 


  writeln( "A more complex 2D locale assignment:" );
  const weirdDomain  : domain(2) = [0..#size, 0..#size];
  // start by emulating cyclic distribution
  var weirdLocales : [(i,j) in weirdDomain] locale = Locales( (i * colcount + j) % numLocales );
  // then update center to point elsewhere
  [ij in [2..5, 2..5] ] weirdLocales(ij) = Locales(0); // change center
  printBlockLocaleAssignments(weirdLocales);

  writeln("done.");
}
  
