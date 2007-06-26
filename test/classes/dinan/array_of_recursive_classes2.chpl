class A {
  var x: int;
}

class B {
  var a: A;
}


var b: B();
b.a = A(10);

writeln("success");
