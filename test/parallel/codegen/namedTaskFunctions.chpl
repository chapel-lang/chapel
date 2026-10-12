

iter customIter() {
  foreach i in 1..10 {
    yield i;
  }
}
iter customIter(param tag) where tag == iterKind.standalone {
  coforall i in 1..10 {
    yield i;
  }
}

proc testCoforall() {
  var sum: atomic int;
  coforall i in 1..10 {
    sum.add(i);
  }
  writeln("testCoforall: ", sum.read());
}
proc testForeach() {
  var sum: atomic int;
  foreach i in 1..10 {
    sum.add(i);
  }
  writeln("testForeach: ", sum.read());
}
proc testFor() {
  var sum: atomic int;
  for i in 1..10 {
    sum.add(i);
  }
  writeln("testFor: ", sum.read());
}
proc testForall() {
  var sum: atomic int;
  forall i in 1..10 {
    sum.add(i);
  }
  writeln("testForall: ", sum.read());
}
proc testBracket() {
  var sum: atomic int;
  [i in 1..10] {
    sum.add(i);
  }
  writeln("testBracket: ", sum.read());
}
proc testCustomIter() {
  var sum: atomic int;
  for i in customIter() {
    sum.add(i);
  }
  writeln("testCustomIter with for: ", sum.read());
  sum.write(0);
  foreach i in customIter() {
    sum.add(i);
  }
  writeln("testCustomIter with foreach: ", sum.read());
  sum.write(0);
  forall i in customIter() {
    sum.add(i);
  }
  writeln("testCustomIter with forall: ", sum.read());
  sum.write(0);
  [i in customIter()] {
    sum.add(i);
  }
  writeln("testCustomIter with brackets: ", sum.read());
}

proc testBegin() {
  var sum: atomic int;
  var done: atomic bool = false;
  begin {
    sum.add(1);
    done.write(true);
  }
  done.waitFor(true);
  writeln("testBegin: ", sum.read());
}

proc testCobegin() {
  var sum: atomic int;
  cobegin {
    sum.add(1);
    sum.add(2);
    sum.add(3);
  }
  writeln("testCobegin: ", sum.read());
}

proc testOn() {
  var sum: atomic int;
  on Locales.last {
    sum.add(1);
  }
  writeln("testOn: ", sum.read());
}
proc testForOn() {
  var sum: atomic int;
  for i in 1..10 {
    on Locales.last {
      sum.add(i);
    }
  }
  writeln("testForOn: ", sum.read());
}

proc testCoforallOn() {
  var sum: atomic int;
  coforall i in 1..10 {
    on Locales.last {
      sum.add(i);
    }
  }
  writeln("testCoforallOn: ", sum.read());
}

proc testBeginOn() {
  var sum: atomic int;
  var done: atomic bool = false;
  begin on Locales.last {
    sum.add(1);
    done.write(true);
  }
  done.waitFor(true);
  writeln("testBeginOn: ", sum.read());
}

proc testCobeginOn() {
  var sum: atomic int;
  cobegin {
    on Locales.last do sum.add(1);
    on Locales.last do sum.add(2);
    on Locales.last do sum.add(3);
  }
  writeln("testCobeginOn: ", sum.read());
}


proc main() {
  testCoforall();
  testForeach();
  testFor();
  testForall();
  testBracket();
  testCustomIter();
  testBegin();
  testCobegin();
  testOn();
  testForOn();
  testCoforallOn();
  testBeginOn();
  testCobeginOn();
  writeln("="*80);
}
