
iter seriter() { for i in 2..3 do yield i*11; }
iter pariter() { for i in 4..5 do yield i*11; }
iter pariter(param tag) { for i in 6..7 do yield i*111; }

proc test(arg) {
  writeln("test of ", arg);
  for s1 in seriter() {
    forall p1 in pariter() {
      if p1 == 666 then continue;
      writeln(p1);
    }
  }

  label named
    for s2 in seriter() {
      forall p2 in pariter() {
        writeln(s2);
        if s2 == 22 then
          continue named;
        writeln("p2");
        if s2 == 33 then
          break named;
        writeln("after break");
      }
    }

  var l$: sync int = 1;
  forall p3 in pariter() {
    const p3n = l$;
    for s3 in seriter() {
      writeln(p3n);
      if s3 == 22 then
        continue;
      writeln(s3);
      if s3 == 33 then
        break;
      writeln("after break");
    }
    l$ = p3n + 1;
  }

  writeln("done");
}

test(9.99);
test("hi");
