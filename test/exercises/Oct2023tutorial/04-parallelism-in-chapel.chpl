// From the Parallelism Supported by Chapel slide in deck 04

var numTasks = 10;

coforall loc in Locales do on loc { /* ... */ }
coforall tid in 0..<numTasks { /* ... */ }

cobegin { /*doTask0(); doTask1(); ... doTaskN();*/ }

var x : atomic int = 0, y : sync int;
sync {
  begin x.add(1);
  begin y.writeEF(1);
  begin x.sub(1);
  begin { y.readFE(); y.writeEF(0); }
}
assert(x.read() == 0);
assert(y.readFE() == 0);

var n = [i in 1..10] i*i;
forall x in n do x += 1;

var nPartialSums = + scan n;
var nSum = + reduce n;

