class C {
  proc chpl_destroy() {
    writeln("chpl_destroy");
  }
}
proc main {
  var c = new unmanaged C();
  delete c;
}
