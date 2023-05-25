use CommUtil;


config const n = 100000;
var A: [1..n] int;

start();

on Locales[1] {
  for i in 1..n {
    A[i] = 1;
  }
}

stop();

writeln(A[1]);
writeln(A[n]);

report(maxGets=maxGets(), maxOns=1, maxGetsToAdjust=1);

proc maxGets() {
  use ChplConfig;

  // accept higher comm counts on m1 mac for now
  // see https://github.com/Cray/chapel-private/issues/3728
  if CHPL_TARGET_PLATFORM == "darwin" && CHPL_TARGET_ARCH == "arm64"
  then return 2;
  
  return 1;
}
