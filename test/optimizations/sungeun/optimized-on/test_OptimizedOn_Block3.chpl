config const n = 10;
config const l = n/2;
use BlockDist;

var Dist = new dmap(new Block(boundingBox={1..n}));
var Dom: domain (1) dmapped Dist = {1..n};
var A: [Dom] 2*int = (1,1);

on Dist.idxToLocale(l) do {
  local {
    A(l) = (l,l);
  }
 }

writeln(A);
