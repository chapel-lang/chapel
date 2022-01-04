record R {
  var x;
}

proc otherOutFn(input, out output: R) {
  output = new R(input); // establishes type of output!
  writeln("in otherOutFn, output has type ", output.type:string);
}

proc outFn(input, out output: R) {
  otherOutFn(input, output); // establishes type of output!
  writeln("in outFn, output has type ", output.type:string);
}

proc test1() {
  var x: R; // generic type!
  outFn(1, x); // sets type of x! 
  writeln("x has type ", x.type:string, " and value ", x);
}
test1();

proc test2() {
  var x; // no type!
  outFn(2, x); // sets type of x! 
  writeln("x has type ", x.type:string, " and value ", x);
}
test2();
