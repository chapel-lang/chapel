
iter seriter() { for i in 2..3 do yield i*11; }
iter pariter() { for i in 4..5 do yield i*11; }
iter pariter(param tag) { for i in 6..7 do yield i*111; }

proc test(arg) {
  writeln("test of ", arg);

  for s1 in seriter() {
    forall p1 in pariter() {
      writeln(s1);
      if p1 == 666 then continue;
      writeln(p1);
    }
  }

  writeln("---");

  label named
    for s2 in seriter() {
      forall p2 in pariter() {
        writeln(s2,p2);
        continue;
      }
    }

  writeln("---");

  forall p3 in pariter() {
    for s3 in seriter() {
      writeln(p3);
      if s3 == 22 then
        continue;
      writeln(s3);
      if s3 == 33 then
        break;
      writeln("after break");
    }
  }

  writeln("done");
}

test(9.99);
test("hi");
