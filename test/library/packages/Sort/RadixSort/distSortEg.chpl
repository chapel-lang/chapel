use BlockDist;
use Random;
use Sort;
use simpleMerge;

config const n = 10;
 
proc main() {
  const Space = {1..n};
  const BlockSpace = Space dmapped Block(boundingBox=Space);

  var BA: [BlockSpace] int ;
  var indicesArr: [1..0] int;
  fillRandom(BA);

  for loc in Locales do on loc {
    const indices = BA.localSubdomains();
   indicesArr.push_back(indices.low);
  }

  coforall loc in Locales {
    on loc do{
     for s in BA.localSubdomains(){
     sort(BA[s]);
     }
    }
  }

  writeln(isSorted(BA));
}

