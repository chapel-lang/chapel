class MyClass { var x:int; }

record Collection {
  var element;
}

pragma "unsafe" // TODO
proc Collection.addElement(arg: element.type) {
  element = arg;
}

proc test() {
  var c: Collection(owned MyClass);
  c.addElement( new owned MyClass() ); // transferred to element

  var global = new owned MyClass();

  var d: Collection(int);     d.addElement( 1 ); // OK
  var e: Collection(MyClass); e.addElement(global.borrow()); // OK
}

test();
