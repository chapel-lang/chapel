class C {
  proc deinit() {
    writeln("C.deinit");
  }
}
proc main() {
  var x = new C();
  writeln(x.type:string, " ", x);
}
