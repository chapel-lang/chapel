

record R {
  var field: int;
}

proc run() {
  var x = new owned R(1);
  //writeln(x.borrow());
}

run();
