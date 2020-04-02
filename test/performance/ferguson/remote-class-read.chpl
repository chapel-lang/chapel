use CommUtil;

config const n = 100000;
class C {
  var x:int;
  var y:int;
  var z:int;
}

var A:[1..n] owned C = for i in 1..n do new owned C(i, i+1, i+2);

var toOutput = 0;

start();

on Locales[1] {
  var sum = 0;
  for c in A {
    sum += c.x;
    sum += c.y;
    sum += c.z;
  }
  toOutput = sum;
}

stop();

writeln(toOutput);
writeln(A[1]);
writeln(A[n]);

report(maxPuts=1, maxOns=1);
