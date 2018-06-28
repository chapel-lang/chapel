class C {
  proc deinit() { writeln("In deinit #1"); }
}
proc C.deinit() { writeln("In deinit #2"); }

proc main {
  var c = new unmanaged C();
  delete c;
}
