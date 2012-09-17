class A {
  var x:int;
}

class B : A {
  var y:int;
}

proc main() {
  var a:A = new B();
  var b:B = a:B;
  if b {
    __primitive("chpl_error", "OK");
  } else {
    __primitive("chpl_error", "FAIL");
  }
}

