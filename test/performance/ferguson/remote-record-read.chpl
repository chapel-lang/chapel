use CommUtil;

config const n = 100000;
record C {
  var x:int;
  var y:int;
  var z:int;
}

var A:[1..n] C;
for i in 1..n {
  A[i] = new C(i, i+1, i+2);
}

var totalToPrint = 0;

start();

on Locales[1] {
  var sum = 0;
  for c in A {
    sum += c.x;
    sum += c.y;
    sum += c.z;
  }
  totalToPrint = sum;
}

stop();

writeln(totalToPrint);
writeln(A[1]);
writeln(A[n]);

report(maxPuts=1, maxOns=1);
