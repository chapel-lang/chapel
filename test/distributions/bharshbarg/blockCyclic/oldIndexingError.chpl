use BlockCycDist;

var Dom = {1..8, 1..9, 1..4};
var Space = Dom dmapped BlockCyclic(startIdx=Dom.low, blocksize=(2, 3, 2));

var A : [Space] int;
A = 1;
