var x = foo();
var y = -1;

function main(argc: integer) {
  var y = 42;

  writeln("x is: ", x);
  writeln("y is: ", y);  
}

function foo() {
  writeln("If we see this, I think it worked");
  return 2;
}
