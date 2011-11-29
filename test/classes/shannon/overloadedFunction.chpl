proc foo(x: real) { 
  writeln("and got the real foo.");

}

class C { 
  var x : int = 0;

}

var myC = new C();

proc foo(c: C = myC, fmt: string = "foo", x: int) {
  writeln("and got the int foo.");

}

writeln("Calling the overloaded foo function with an int argument,");
foo(3);

delete myC;
