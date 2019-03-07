

record R {
  var field: int;
}

proc run() {
  var x = new shared R(1);
  //writeln(x.borrow());
}

run();
