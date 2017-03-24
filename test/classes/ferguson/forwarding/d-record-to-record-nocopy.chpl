
record Wrapper {
  var c; // e.g. some class
  forwarding c;
}

record C {
  var field:int;
}

proc run() {
  var r = new Wrapper(new C());

  r.c.field = 1;
  r.field   = 2;
  writeln("r.c.field ", r.field);
  writeln("r.field ", r.c.field);
}


run();
