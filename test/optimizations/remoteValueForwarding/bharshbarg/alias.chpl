
record R {
  var x : int;
}

class C {
  var x : int;
}

var g : int = 0;

var gr : R;

var gc : owned C = new owned C();

proc test(const ref arg) {
  writeln("Testing int");
  on Locales[numLocales-1] {
    writeln("before: ", arg);
    g += 1;
    writeln("after:  ", arg);
    if arg != g then writeln("ERROR: g != arg. g = ", g);
  }
}

proc testRecord(const ref arg) {
  writeln("Testing record");
  on Locales[numLocales-1] {
    writeln("before: ", arg);
    gr.x += 1;
    writeln("after: ", arg);
    if arg.x != gr.x then writeln("ERROR: ", gr, " != ", arg);
  }
}

proc testClass(const ref arg) {
  writeln("Testing class");
  on Locales[numLocales-1] {
    writeln("before: ", arg);
    gc.x += 1;
    writeln("after: ", arg);
    if arg.x != gc.x then writeln("ERROR: ", gc, " != ", arg);
  }
}

test(g);
testRecord(gr);
testClass(gc);
