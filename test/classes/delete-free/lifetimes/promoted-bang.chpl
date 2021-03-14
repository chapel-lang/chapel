module mymodule {
  
  config const i = 1;
  config const j = 1;
  config const n = 2;

  class MyClass {
    var x: int;
  }

  proc setupArrays(A, B, instance) lifetime A < instance, B < instance, B < A {
    A[1] = instance;
    A[2] = A[1];
    B[1] = A[1];
    B[2] = B[1];
  }

  proc setupArray(Arr) {
    var ret:[1..n] borrowed MyClass = Arr!;
    return ret;
  }

  proc main() {
    var Aq:[1..n] borrowed MyClass?;
    var Bq:[1..n] borrowed MyClass?;

    var instance = new borrowed MyClass(0);
    setupArrays(Aq, Bq, instance);

    var A:[1..n] borrowed MyClass = Aq!;
    var B:[1..n] borrowed MyClass = Bq!;

    writeln("OK");
  }
}
