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
  var myOwned = new owned MyClass();
  var b = myOwned.borrow();
  var e: Collection(MyClass); e.addElement(b); // errors
}

test();
