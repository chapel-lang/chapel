proc foo(a: int) {
  class C {
    var x: int;
  }

  var myC = new C();

  writeln("myC is: ", myC);

  delete myC;
}

var a: int;
foo(a);
