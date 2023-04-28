class MyClass { }

proc testShared() {
  var mine = (new shared MyClass()).borrow();
  var x = shared.adopt(mine);
}
testShared();
