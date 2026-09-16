union U {
  var x: int;
  var y: real;
  var z: string;
  var w: int;
}
proc test9() {
  var arr: [1..10] U;
  writeln("bare union in indexing");
  union select arr[4] { // doesn't work, can be worked around with ref local
    when x do x += 1;
    when y do y += 1.0;
    otherwise writeln("not an int or real");
  }
  writeln(arr[4]);
  arr[4].x = 10;
  writeln(arr[4]);
  union select arr[4] {
    when x do x += 1;
    when y do y += 1.0;
    otherwise writeln("not an int or real");
  }
  writeln(arr[4]);
}
test9();
