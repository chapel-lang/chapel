use CommUtil;

config const n = 100000;
record C {
  var x:real;
  var y:real;
  var z:real;
}

var c = new C(1.0,2.0,3.0);

var totalToPrint = 0;

start();


on Locales[1] {
  var sum = 0;
  for x in 1..n {
    // We really hope to see the access to c.x
    // loop-invariant-code-motioned.
    sum += c.x:int;
  }
  totalToPrint = sum;
}

stop();

writeln(totalToPrint);

report(maxPuts=1, maxOns=1);
