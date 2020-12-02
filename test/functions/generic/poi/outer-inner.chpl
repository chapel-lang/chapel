// This example exposes POI-visibility of resolution candidates
// in the case of nested generics.

proc outer(param p) {
  proc inner(param p) {
    writeln("starting inner()");
    xtra();     // which declarations of xtra() are visible here?
    onlyone();  // ensure that outer's POI is also visited
  }
  {
    proc xtra() { writeln("xtra-1"); }
    writeln("starting outer()");
    inner(0);
  }
}
{
  proc xtra() { writeln("xtra-2"); }
  proc onlyone() { writeln("onlyone"); }
  writeln("starting main");
  outer(0);
}
