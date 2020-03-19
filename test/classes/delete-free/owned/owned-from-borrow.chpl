class MyClass { }

proc testOwned() {
  var mine = (new owned MyClass()).borrow();
  var x = owned.create(mine);
}
testOwned();
