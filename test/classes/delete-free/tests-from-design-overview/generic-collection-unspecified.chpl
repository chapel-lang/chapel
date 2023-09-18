class MyClass { var x:int; }

record Collection {
  type eltType;
  var element: eltType;
}

proc ref Collection.addElement(in arg: element.type) lifetime this < arg {
  element = arg;
}

proc test() {
  var c: Collection(owned MyClass?);
  c.addElement( new owned MyClass() ); // transferred to element

  var global = new owned MyClass();

  var d: Collection(int);      d.addElement( 1 ); // OK
  var e: Collection(borrowed MyClass?); e.addElement(global.borrow()); // OK
}

test();
