class MyClass {
  var x:int;
  proc deinit() {
    writeln("in MyClass.deinit");
  }
}

proc test1() {
  writeln("test1");
  var x = new owned MyClass();
  // The following are equivalent ways of declaring a borrow from x:
  var b1 = x.borrow();
  var b2: MyClass = x.borrow();
  var b3 = x: borrowed MyClass; // Cast to borrow
  var b4: borrowed MyClass = x; // Coerce to borrow

  writeln(b1.type:string);
  writeln(b2.type:string);
  writeln(b3.type:string);
  writeln(b4.type:string);
}
test1();

proc computeA(input: MyClass) {
  writeln("in computeA ", input.type:string, " ", input);
}
// Could be written as
proc computeB(input: borrowed MyClass) {
  writeln("in computeB ", input.type:string, " ", input);
}

proc test2a() {
  writeln("test2a");
  var x = new owned MyClass();
  computeA(x); // Coerces to borrow to pass argument
  writeln("back in test2a ", x);
}
test2a();

proc test2b() {
  writeln("test2b");
  var x = new owned MyClass();
  computeB(x); // Coerces to borrow to pass argument
  writeln("back in test2b ", x);
}
test2b();
