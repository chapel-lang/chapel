var mylocdom: domain(2);
var mylocs: [mylocdom] locale;
var manylocs: bool;
config const s1=2, s2=2;
const mygriddom = {0..#s1, 0..#s2};
const mygridlocs: [mygriddom] locale = Locales(0);

// Initialize 'mylocs' and 'manylocs'.
//
/*
proc setupLocales(s1:int, s2:int, ensureManyLocs: bool = false) {
  mylocdom = {0..#s1,0..#s2};
  manylocs = (numLocales >= mylocs.numElements);

  if manylocs {
    var i = 0;
    for ml in mylocs { ml = Locales(i); i += 1; }
  } else {
    mylocs = Locales(0);
  }

  if !manylocs && ensureManyLocs then halt("not enough locales: wanted ",
    mylocs.numElements, ", got ", numLocales);
}
*/

use CyclicDist;
config const n=500;
var Space = {1..n};
const Dom1: domain(1) dmapped Cyclic(startIdx=Space.low)=Space;

var A:[Dom1] real(64);
var C:[Dom1] real(64);

var H: [1..n/2] real(64);
var F: [1..n/2] real(64);

var D6 = {1..10 by 2};
H[D6] = C[D6];
for (a,b) in zip(H[D6],C[D6]) do if (a!=b) then writeln("ERROR!!!!");
writeln();
