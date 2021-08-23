// this module name is hard-coded in the compiler
module GPUOutlineTest {
  extern proc chpl_gpu_init(): void;

  config const start = 1;
  config const end = 10;

  proc testMain() {
    on here.getChild(1) {
      chpl_gpu_init();
      var a, b: [start..end] int;
      var value = 20;

      // one array
      forall  elem in a { elem += 10;          } writeln(a);
      forall  elem in a { elem += value;       } writeln(a);
      foreach elem in a { elem += 10;          } writeln(a);
      foreach elem in a { elem += value;       } writeln(a);
    }
  }
  testMain();
}
