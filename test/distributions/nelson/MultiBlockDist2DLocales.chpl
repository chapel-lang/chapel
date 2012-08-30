
use BlockDist;

config const size : int = 8;
const sizeRange = 0..#(size : int(64));


proc printBlockLocaleAssignments(locs) {
  const blockDist = new dmap(new Block(rank=2, idxType=int(64),
                                                targetLocales=locs,
                                                boundingBox={sizeRange,sizeRange}));
  const blockDom : domain(2, int(64)) dmapped blockDist = {sizeRange, sizeRange};

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

  writeln( "Emulate cyclic distribution by repeating locales in 1D array:" );
  // warning: creates one task per locale assignment; may create too many threads
  const oversubscribed : int = size*size;
  const cyclicLocales : [0..#oversubscribed] locale = [loc in 0..#oversubscribed] Locales(loc % numLocales);
  printBlockLocaleAssignments(cyclicLocales); 


  writeln( "A more complex 2D locale assignment:" );
  const weirdDomain  : domain(2) = {0..#size, 0..#size};
  // start by emulating cyclic distribution
  var weirdLocales : [weirdDomain] locale = [(i,j) in weirdDomain] Locales( (i * colcount + j) % numLocales );
  // then update center to point elsewhere
  [ij in {2..5, 2..5} ] weirdLocales(ij) = Locales(0); // change center
  printBlockLocaleAssignments(weirdLocales);

  writeln("done.");
}
  
