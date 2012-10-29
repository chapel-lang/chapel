class A {
  var x:int;
}

class B : A {
  var y:int;
}

proc main() {
  var a = new A();
  var b:B = a:B;
  if b {
    __primitive("chpl_error", "FAIL");
  } else {
    __primitive("chpl_error", "OK");
  }
}

