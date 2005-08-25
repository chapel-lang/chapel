iterator basic(inout z: integer) : integer {
  var i : integer;
  z += 1;
  while i < 10 {
    yield i * 4;
    i += 1;
  }
}



function main() {
var x : integer = 5;
var a: integer = 9;
writeln("initially, a is: ", a);
  for i in basic(a) {
    writeln(i+x);
  } 

  writeln("after first iteration, a is: ", a);

  for j in basic(a) {
     var y : integer = 4;
     x = 3;
     writeln(j + y + x);
  }

  writeln("after second iteration, a is: ", a);

  writeln("after iteration, x is: ", x);
}