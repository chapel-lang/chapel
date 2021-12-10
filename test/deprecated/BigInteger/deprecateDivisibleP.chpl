use BigInteger;

var a=new bigint(4);
var b = new bigint( 2);
var c:int;
var d:int;

c=a.divisible_p(d=b);
d=a.divisible_p(d=3);
writeln(a," is divisible by ",b,", as ",c," was returned");
writeln(a," is not divisible by 3, as ",d," was returned");
