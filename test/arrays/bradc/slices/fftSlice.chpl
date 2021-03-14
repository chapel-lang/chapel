param radix = 4;

config const n: int(64) = 6;
const m = 2**n;

proc main() {
  const ProblemDom: domain(1, int(64)) = {0:m.type..#m};
  var Z: [ProblemDom] complex(128);

  dfft(Z);
}

proc dfft(A: [?ADom]) {
  var str = 2;
  var span = 8;
  var wk1, wk2, wk3: complex(128);

  writeln("ADom is: ", ADom);
  for bankStart in ADom by 2*span {
    writeln("bankStart is: ", bankStart);
    for lo in bankStart + (0..#str) {
      writeln("lo is: ", lo);
      var t1 = 0..#radix;
      writeln("t1 is: ", t1);
      var t2 = 0.. by str #radix;
      writeln("t2 is: ", t2);
      var t3 = lo.. by str #radix;
      writeln("t3 is: ", t3);
      var B = A[lo.. by str # radix];
      writeln("B is: ", B);
      //      butterfly(wk1, wk2, wk3, A[[0..radix)*str + lo]);
    }
  }
}

proc butterfly(wk1, wk2, wk3, inout A) {
  writeln("A is: ", A);
}
