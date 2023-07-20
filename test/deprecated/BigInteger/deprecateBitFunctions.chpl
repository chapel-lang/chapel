// deprecated in 1.32 by Jade
use BigInteger;
use IO.FormattedIO;

var x: bigint = 0xFF;
writef("0x%xu\n", x:int);

x.clrbit(0);
writef("0x%xu\n", x:int);

x.combit(2);
writef("0x%xu\n", x:int);

x.setbit(9);
writef("0x%xu\n", x:int);

var i = x.tstbit(8);
writeln(i);
