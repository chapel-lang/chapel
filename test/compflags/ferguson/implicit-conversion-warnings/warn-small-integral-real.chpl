use Math;

proc test1fn() {
  var red: uint(8) = 1;
  var green: uint(8) = 2;
  var blue: uint(8) = 3;
  return 0.2126*red + 0.7152*green + 0.0722*blue;
}

proc test1() {
  var x = test1fn();
  writeln("test1fn() = ", x, " : ", x.type:string);
}
test1();

proc test2() {
  var myInt16: int(16) = 1;
  var result = sin(myInt16);
  writeln("sin(myInt16) = ", result, " : ", result.type:string);
}
test2();

proc test3() {
  var x: int(16) = 1;
  var y = 2.0;
  var z  = sqrt(x) + sin(y);
  writeln("z = ", z, " : ", z.type:string);
}
test3();

proc test4() {
  param two: uint(16) = 2;
  var x = sqrt(two);
  writeln("sqrt(2:uint(16)) = ", x, " : ", x.type:string);
}
test4();

proc test5fn(arg: real(32)) { }

proc test5() {
  test5fn(1:int(16));
}
test5();

proc test6fn(arg: real(32)) param { return 1.0:real(32); }

proc test6() {
  var x = test6fn(1:int(16));
  writeln("test6 ", x, " : ", x.type:string);
}
test6();

proc test7fn(arg: real(32)) { }

proc test7() {
  var myInt32: int(32);
  test7fn(myInt32);
}
test7();
