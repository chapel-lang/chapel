class MyClass { var x:int; }

record Collection {
  var element: owned;
}

proc Collection.addElement(in arg: owned) {
  element = arg;
}

proc test() {
  var c: Collection(owned MyClass?);
  c.addElement(new owned MyClass()); // transferred to element
}

test();
