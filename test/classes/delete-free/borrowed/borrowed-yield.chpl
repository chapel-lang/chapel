class MyClass {
  var x: int;
}

iter myiter() {
  var oYield = new owned MyClass();
  yield oYield.borrow();
}


proc test() {
  for x in myiter() {
    writeln(x.type:string, " ", x);
  }
}

test();
