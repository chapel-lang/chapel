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
  var c: Collection(MyClass);
  c.addElement(b); // collection borrows global
  c.addElement(new owned MyClass()); // collection borrows new owned class 
}

test();
