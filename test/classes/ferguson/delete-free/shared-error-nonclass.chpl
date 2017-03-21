use SharedObject;

record R {
  var field: int;
}

proc run() {
  var x = new Shared(new R(1));
  writeln(x.borrow());
}

run();
