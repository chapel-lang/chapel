// Test nil checking in forall loop bodies

class MyClass {
  var x: int;
  proc method() {
    writeln("in method");
  }
}

config const n = 0;

proc badNilInForallLoop() {
  var x:owned MyClass? = new owned MyClass(1);
  forall i in 1..n with (ref x) {
    // yes this is a race condition
    x = nil;
    x!.method();
  }
}
badNilInForallLoop();

proc badNilInCoforallLoop() {
  var x:owned MyClass? = new owned MyClass(1);
  coforall i in 1..n with (ref x) {
    // yes this is a race condition
    x = nil;
    x!.method();
  }
}
badNilInCoforallLoop();


proc badNilInForallLoop2() {
  forall i in 1..n {
    var x:owned MyClass?;
    x!.method();
  }
}
badNilInForallLoop2();
