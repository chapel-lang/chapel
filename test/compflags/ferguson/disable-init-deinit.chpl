record R {
  proc init() {
    writeln("init");
  }
  proc deinit() {
    writeln("deinit");
  }
}

proc main() {
  writeln("before declaration");
  var r: R;
  writeln("after declaration, before assignment");
  r = new R();
  writeln("after assignment");
}
