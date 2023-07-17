class MyClass { }

proc testShared() {
  var obj = new shared MyClass(); var mine = obj.borrow();
  var x = shared.adopt(mine);
}
testShared();
