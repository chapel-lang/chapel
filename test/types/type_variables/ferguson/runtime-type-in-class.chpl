class C {
  type t;

  var x: t;
}

proc run() {
  var D: domain(int);
  D += 6;

  // now D is {6}

  type arrType = [D] real;
  var c = new borrowed C(arrType);

  // now c.x.domain is {6}

  D += 2;
  D -= 6;
  writeln(c.x.domain);
  // should it output {2} or {6} ?
  writeln(c);
}

run();
