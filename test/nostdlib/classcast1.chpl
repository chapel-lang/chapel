class A {
  var x:int;
}

class B : A {
  var y:int;
}

proc main() {
  var a = new A();
  var b:B = a:B; // fail here.
  var y = b.y;
  var str = y:string;
  __primitive("chpl_error", str);
}

