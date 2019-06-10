class MyClass { }

proc testShared() {
  var mine = (new shared MyClass()).borrow();
  var x = new shared(mine);
}
testShared();
