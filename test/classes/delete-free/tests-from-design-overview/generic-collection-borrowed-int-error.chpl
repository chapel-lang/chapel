class MyClass { var x:int; }

record Collection {
  var element: borrowed;
}

proc Collection.addElement(arg: borrowed) {
  element = arg;
}

proc test() {
  var myOwned = new owned MyClass();
  var b = myOwned.borrow();
  var d: Collection(int); d.addElement(1); // errors
}

test();
