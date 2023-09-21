class MyClass { }

proc testOwned() {
  var obj = new MyClass(); var mine = obj.borrow();
  var x = owned.adopt(mine);
}
testOwned();
