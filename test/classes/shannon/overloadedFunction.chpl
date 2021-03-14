proc foo(x: real) { 
  writeln("and got the real foo.");

}

class C { 
  var x : int = 0;

}

var myC = new borrowed C();

proc foo(c: borrowed C = myC, fmt: string = "foo", x: int) {
  writeln("and got the int foo.");

}

writeln("Calling the overloaded foo function with an int argument,");
foo(3);
