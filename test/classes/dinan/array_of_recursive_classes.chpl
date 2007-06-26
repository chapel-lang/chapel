class A {
  var x: int;
  var b: [1..x] B;
}

class B {
  var a: A;   // This produces an error
//var a: A(); // This seems to work, but produces the seg faulting
              // binary seen in the future with the same name + 2
}


var b: B();
b.a = A(10);

writeln("success");
