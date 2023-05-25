use BigInteger;
use OS;

var b1 = new bigint("303948673745");
writeln(b1);
var err: errorCode;
var b2 = new bigint("373724632625625626523", error=err);
writeln(b2);
writeln(err == 0);
