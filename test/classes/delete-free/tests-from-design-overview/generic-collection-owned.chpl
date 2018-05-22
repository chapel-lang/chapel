pragma "use default init"
class MyClass { var x:int; }

pragma "use default init"
record Collection {
  var element: owned;
}

proc Collection.addElement(arg: owned) {
  element = arg;
}

proc test() {
  var c: Collection(owned MyClass);
  c.addElement(new owned MyClass()); // transferred to element
}

test();
