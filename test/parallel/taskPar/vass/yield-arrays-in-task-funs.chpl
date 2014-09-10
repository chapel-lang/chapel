// Test of yielding arrays from task functions.

const P = {1..1};

iter normlIter(param tag: iterKind) {
  var p: [P]int;
  p[1] = 111;
  yield p;
}

iter rtValIter(param tag: iterKind) : [P]int where tag == iterKind.leader {
  var p: [P]int;
  p[1] = 111;
  cobegin {
    yield p;
    yield p;
  }
  on Locales[numLocales-1] {
    yield p;
    yield p;
  }
}

iter rtRefIter(param tag: iterKind) ref : [P]int where tag == iterKind.leader {
  var p: [P]int;
  p[1] = 111;
  cobegin {
    yield p;
    yield p;
  }
  on Locales[numLocales-1] {
    yield p;
    yield p;
  }
}

iter ntValIter(param tag: iterKind) where tag == iterKind.leader {
  var p: [P]int;
  p[1] = 111;
  cobegin {
    yield p;
    yield p;
  }
  on Locales[numLocales-1] {
    yield p;
    yield p;
  }
}

iter ntRefIter(param tag: iterKind) ref where tag == iterKind.leader {
  var p: [P]int;
  p[1] = 111;
  cobegin {
    yield p;
    yield p;
  }
  on Locales[numLocales-1] {
    yield p;
    yield p;
  }
}

writeln();

for normlVar in normlIter(iterKind.leader) {
  writeln("normlIter ", normlVar[1]);
}
writeln();

for rtValVar in rtValIter(iterKind.leader) {
  writeln("rtValIter ", rtValVar[1]);
}
writeln();

for rtRefVar in rtRefIter(iterKind.leader) {
  writeln("rtRefIter ", rtRefVar[1]);
}
writeln();

for ntValVar in ntValIter(iterKind.leader) {
  writeln("ntValIter ", ntValVar[1]);
}
writeln();

for ntRefVar in ntRefIter(iterKind.leader) {
  writeln("ntRefIter ", ntRefVar[1]);
}
writeln();
