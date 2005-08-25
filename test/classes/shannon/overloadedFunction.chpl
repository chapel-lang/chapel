function foo(x: float) { 
  writeln("and got the float foo.");

}

class C { 
  var x : integer = 0;

}

var myC = C();

function foo(c: C = myC, fmt: string = "foo", x: integer) {
  writeln("and got the integer foo.");

}

writeln("Calling the overloaded foo function with an integer argument,");
foo(3);