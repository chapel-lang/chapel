config const timing = false;
config const iters = 10000;
config const size  = 100000;

use Time;

proc returnArray() {
  var X: [1..size] real;
  for (x,i) in zip(X, 1..size) {
    x = i;
  }
  return X;
}

proc returnArrayTyped():[1..size] real {
  var X: [1..size] real;
  for (x,i) in zip(X, 1..size) {
    x = i;
  }
  return X;
}

proc returnArrayArg(X) {
  for (x,i) in zip(X, 1..size) {
    x = i;
  }
}

proc returnArrayArgTyped(X:[1..size] real) {
  for (x,i) in zip(X, 1..size) {
    x = i;
  }
}

var t1 = new Timer();
var t2 = new Timer();
var t3 = new Timer();
var t4 = new Timer();
var sum0 = 0.0;
var sum1 = 0.0;
var sum2 = 0.0;
var sum3 = 0.0;
var sum4 = 0.0;

writeln("checking array return performance");
writeln("array size: ", size);
writeln("iterations: ", iters);
writeln();

// Warm-up loop
// I noticed that the 1st thing measured is slower
// than the others...
for it in 1..iters {
  var A = returnArray();
  for a in A do sum0 += a;
}


t1.start();

for it in 1..iters {
  var A = returnArray();
  for a in A do sum1 += a;
}

t1.stop();

t2.start();

for it in 1..iters {
  var A = returnArrayTyped();
  for a in A do sum2 += a;
}

t2.stop();


t3.start();

for it in 1..iters {
  var A: [1..size] real;
  returnArrayArg(A);
  for a in A do sum3 += a;
}

t3.stop();

t4.start();

for it in 1..iters {
  var A: [1..size] real;
  returnArrayArgTyped(A);
  for a in A do sum4 += a;
}

t4.stop();

if sum1 == sum2 && sum2 == sum3 && sum3 == sum4 then
  writeln("SUCCESS");
else
  writeln("FAILURE");

writeln("sum1: ", sum1 / iters);
writeln("sum2: ", sum2 / iters);
writeln("sum3: ", sum3 / iters);
writeln("sum4: ", sum4 / iters);

if timing {
  var serpar = "";
  if dataParTasksPerLocale == 1 then
    serpar = "serial ";
  else
    serpar = "parallel ";

  writeln(serpar, size, " untyped return  ", t1.elapsed());
  writeln(serpar, size, " typed return    ", t2.elapsed());
  writeln(serpar, size, " untyped arg     ", t3.elapsed());
  writeln(serpar, size, " typed arg       ", t4.elapsed());
}

