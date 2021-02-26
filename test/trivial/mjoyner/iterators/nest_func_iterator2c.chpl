iter basic(out z: int) : int {
  var i : int;
  z = 99;
  while i < 10 {
    yield i * 4;
    i += 1;
  }
}



proc main() {
var x : int = 5;
var a: int = 9;
writeln("initially, a is: ", a);
  for i in basic(a) {
    writeln(i+x);
  } 

  writeln("after first iteration, a is: ", a);

  for j in basic(a) {
     var y : int = 4;
     x = 3;
     writeln(j + y + x);
  }

  writeln("after second iteration, a is: ", a);

  writeln("after iteration, x is: ", x);
}
