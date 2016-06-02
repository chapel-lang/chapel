

config const n = 10000;
config const r = 1000;

writeln("n=", n, "   r=", r);
var errors = 0;

for rIdx in 1..r {

  var d:domain(int);
  for i in 1..n {  d += i; }
  var expected = n*(n+1)/2;
  var sumd = + reduce d;
  if (sumd != expected) {
    //  writeln(expected, ": ", sumd);
    errors += 1;
  }

} // for rIdx

writeln(errors, " errors");
if errors then exit(2);
