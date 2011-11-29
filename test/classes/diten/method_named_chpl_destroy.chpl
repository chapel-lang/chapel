class C {
  proc chpl_destroy() {
    writeln("chpl_destroy");
  }
}
proc main {
  var c = new C();
  delete c;
}
