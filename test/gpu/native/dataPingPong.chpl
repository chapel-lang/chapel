use Time;

config const n = 10;
config const numOps = 10;
config const printResult = true;
config const printTimes = false;
config const useGPU = true;

var detailTimer = new Timer();
var totalTimer = new Timer();

var A: [1..n] int;

for i in A.domain {
  A[i] = i;
}

var copyIn = 0.0,
    kernel = 0.0,
    copyOut = 0.0,
    total = 0.0;

totalTimer.start();
if useGPU {
  on here.getChild(1) {
    detailTimer.start();
    var B = A;
    detailTimer.stop();
    copyIn = detailTimer.elapsed();
    detailTimer.clear();


    detailTimer.start();

    foreach b in B {
      op(b, numOps);
    }

    detailTimer.stop();
    kernel = detailTimer.elapsed();
    detailTimer.clear();

    on A {
      detailTimer.start(); // technically we should include the on statement to this timing
      A = B;
      detailTimer.stop();
      copyOut = detailTimer.elapsed();
      detailTimer.clear();
    }
  }
}
else {
  detailTimer.start();

  forall a in A do
    op(a, numOps);

  detailTimer.stop();
  kernel = detailTimer.elapsed();
  detailTimer.clear();
}
totalTimer.stop();
total = totalTimer.elapsed();

if printTimes {
  writeln("copyIn: ", copyIn);
  writeln("kernel: ", kernel);
  writeln("copyOut: ", copyOut);
  writeln("total: ", total);
}

if printResult then
  writeln("Array: ", A);

inline proc op(ref val, numOps) {
  for i in 1..numOps {
    val += i*i / (2+i);
  }
}
