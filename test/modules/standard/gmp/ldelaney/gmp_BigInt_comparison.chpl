use GMP;

// Tests the comparison operators

var a = new BigInt(5);
var b = new BigInt(-6);

var val = a.cmp(b);
writeln(a, " ", cmp_parse(val), " ", b);
val = a.cmp_d(5.0);
writeln(a, " ", cmp_parse(val), " 5.0");
val = a.cmp_si(-2);
writeln(a, " ", cmp_parse(val), " -2");
val = a.cmp_ui(6);
writeln(a, " ", cmp_parse(val), " 6");

val = a.cmpabs(b);
writeln("abs(", a, ") ", cmp_parse(val), " abs(", b, ")");
a.neg(a);
val = a.cmpabs_d(-4.2);
writeln("abs(", a, ") ", cmp_parse(val), " abs(-4.2)");
val = a.cmpabs_ui(4);
writeln("abs(", a, ") ", cmp_parse(val), " 4");
val = a.sgn();
if val == 1 then writeln(a, " is positive");
else if val == 0 then writeln(a, " is zero");
else if val == -1 then writeln(a, " is negative");

proc cmp_parse(val){
  var ret : string;
  if val < 0 then
    ret = "<";
  else if val == 0 then
    ret = "=";
  else if val > 0 then
    ret = ">";
  else
   ret = "INVALID";
  return ret;
}
