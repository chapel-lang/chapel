use OwnedObject;

record R {
  var field: int;
}

proc run() {
  var x = new Owned(new R(1));
  writeln(x.borrow());
}

run();
