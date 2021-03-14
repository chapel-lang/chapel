record R {
  var x1: int;
  var x2: int;
  var x3: int;
}

proc main {
  var r: R;
  r.x1 = 17;
  r.x2 = 19;
  r.x3 = 23;
  writeln(here.id, ": ", r.x1, " ", r.x2, " ", r.x3);
  r.x1 = r.x1 + 1;
  r.x2 = r.x2 + 2;
  r.x3 = r.x3 + 3;

  on Locales(1) {
    writeln(here.id, ": ", r.x1, " ", r.x2, " ", r.x3);
    r.x1 = r.x1 + 1;
    r.x2 = r.x2 + 2;
    r.x3 = r.x3 + 3;
  }

  writeln(here.id, ": ", r.x1, " ", r.x2, " ", r.x3);
}
