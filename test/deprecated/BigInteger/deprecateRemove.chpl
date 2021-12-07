use BigInteger;

var a = new bigint( 32);
var b = new bigint( 4);
var x = new bigint( 7);
var c = new bigint();
var d = new bigint();
var p = new bigint();
var q = new bigint();

d=c.remove(a, b);
q=p.removeFactor(a=x,f=b);

writeln("32 is ",c," times the factor ",b," raised to the power ",d);
writeln("7 is ",p," times the factor ",b," raised to the power ",q);
