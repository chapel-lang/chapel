class MyClass { }

proc testOwned() {
  var mine = (new owned MyClass()).borrow();
  var x = new owned(mine);
}
testOwned();
