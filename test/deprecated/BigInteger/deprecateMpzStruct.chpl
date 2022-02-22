use BigInteger;
use GMP;

config const debug = false;

var a: bigint = 14;
var impl = a.mpzStruct();
var ret = chpl_gmp_mpz_nlimbs((impl,));
if debug then writeln(ret);
