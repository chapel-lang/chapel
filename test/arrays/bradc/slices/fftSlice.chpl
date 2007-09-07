param radix = 4;

config const n: int(64) = 16;
const m = 2**n;

def main() {
  const ProblemDom: domain(1, int(64)) = [0..m);
  var Z: [ProblemDom] complex(128);

  dfft(Z);
}

def dfft(A: [?ADom]) {
  var str = 2;
  var span = 8;
  var wk1, wk2, wk3: complex(128);

  for bankStart in ADom by 2*span {
    for lo in bankStart + [0..str) {
      var t1 = [0..radix);
      writeln("t1 is: ", t1);
      var t2 = [0..radix)*str;
      writeln("t2 is: ", t2);
      var t3 = [0..radix)*str + lo;
      writeln("t3 is: ", t3);
      var B = A[[0..radix)*str + lo];
      writeln("B is: ", B);
      //      butterfly(wk1, wk2, wk3, A[[0..radix)*str + lo]);
    }
  }
}

def butterfly(wk1, wk2, wk3, inout A) {
  writeln("A is: ", A);
}
