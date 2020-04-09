var x = foo();
var y = -1;

proc main(argc: int, argv: [1..argc] string) {
  var y = 42;

  writeln("x is: ", x);
  writeln("y is: ", y);  
}

proc foo() {
  writeln("If we see this, the program ran");
  return 0;
}
