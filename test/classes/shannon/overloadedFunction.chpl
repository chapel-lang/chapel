fun foo(x: float) { 
  writeln("and got the float foo.");

}

class C { 
  var x : int = 0;

}

var myC = C();

fun foo(c: C = myC, fmt: string = "foo", x: int) {
  writeln("and got the int foo.");

}

writeln("Calling the overloaded foo fun with an int argument,");
foo(3);
