
var g : int = 0;

var gr : R;

record R {
  var x : int;
}

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

test(g);
testRecord(gr);
