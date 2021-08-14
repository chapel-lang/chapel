// this module name is hard-coded in the compiler
module GPUOutlineTest {
  extern proc chpl_gpu_init(): void;

  proc testMain() {
    on here.getChild(1) {
      chpl_gpu_init();
      var a, b: [0..10] int;
      var value = 20;

      // one array
      forall  i in 0..0 { a[i] += i+10;       } writeln(a);
      forall  i in 0..0 { a[i] += i+value;    } writeln(a);
      foreach i in 0..0 { a[i] += i+10;       } writeln(a);
      foreach i in 0..0 { a[i] += i+value;    } writeln(a);

      // two arrays
      forall  i in 0..0 { b[i] += a[i]*10;    } writeln(b);
      forall  i in 0..0 { b[i] += a[i]*value; } writeln(b);
      foreach i in 0..0 { b[i] += a[i]*10;    } writeln(b);
      foreach i in 0..0 { b[i] += a[i]*value; } writeln(b);
    }
  }
  testMain();
}
