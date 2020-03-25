record R {
  var x: int;
  proc init() {
    writeln("default init");
  }
  proc init(arg: int) {
    writeln("init ", arg);
    this.x = arg;
  }
  proc deinit() {
    writeln("deinit ", x);
  }
}

proc makeR(arg: int) {
  return new R(arg);
}

proc f(arg) { }

proc main() {
  writeln("before declaration");
  var r: R;
  writeln("after declaration, before assignment");
  r = makeR(1);
  writeln("after assignment");
  writeln("before nested call");
  f(makeR(2));
  writeln("after nested call");
}
