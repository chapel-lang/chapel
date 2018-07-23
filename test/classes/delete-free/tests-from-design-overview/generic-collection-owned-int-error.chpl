class MyClass { var x:int; }

record Collection {
  var element: owned;
}

proc Collection.addElement(arg: owned) {
  element = arg;
}

proc test() {
  var d: Collection(int); d.addElement(1); // errors
}

test();
