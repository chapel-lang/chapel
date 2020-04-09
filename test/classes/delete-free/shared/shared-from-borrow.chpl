class MyClass { }

proc testShared() {
  var mine = (new shared MyClass()).borrow();
  var x = shared.create(mine);
}
testShared();
