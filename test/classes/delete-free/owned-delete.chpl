use OwnedObject;

class MyClass {
  var x: int;
}

proc main() {
  var myowned = new Owned(new MyClass(1));

  writeln(myowned.borrow());

  delete myowned;
}
