use BigInteger;

var a=new bigint(4);
var b = new bigint(3);
var c:integral=8;
var d:integral=3;
var n=new bigint(7);
var m=new bigint(9);

var x:bool=n.congruent_p(c=a,d=b);
var y:bool=m.congruent_p(c=c,d=d);
writeln(n," is congruent to 4 mod 3, as ",x," was returned");
writeln(m," is not congruent to 8 mod 3, as ",y," was returned");
