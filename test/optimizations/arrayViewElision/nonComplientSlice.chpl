// for now, we don't support distributed arrays sliced with distributed domains
use BlockDist;

config const n = 10;

var dom = blockDist.createDomain(1..n);
var innerDom = dom.expand(-1);

var Arr: [dom] int;
var Crr: [dom] int = 1;

Arr[innerDom] = Crr[innerDom];

writeln(Arr);




