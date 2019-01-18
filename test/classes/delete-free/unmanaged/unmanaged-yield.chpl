class MyClass {
  var x: int;
}

iter myiter() {
  yield new unmanaged MyClass();
}


proc test() {
  for x in myiter() {
    writeln(x.type:string, " ", x);
    delete x;
  }
}

test();
