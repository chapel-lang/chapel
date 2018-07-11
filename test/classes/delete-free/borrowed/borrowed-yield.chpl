class MyClass {
  var x: int;
}

iter myiter() {
  yield new borrowed MyClass();
}


proc test() {
  for x in myiter() {
    writeln(x.type:string, " ", x);
  }
}

test();
