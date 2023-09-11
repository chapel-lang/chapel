use BlockDist;
use BigInteger;

var a = blockDist.createArray({1..10}, bigint);
forall i in a.domain with (ref a) do a[i] = i:bigint;
var res = + scan a;

writeln(res);
