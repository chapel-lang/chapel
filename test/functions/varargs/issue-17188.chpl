proc f( x:int ...?k, named:int) {
  writeln("f with named");
}
proc f( x:int ...?k) {
  writeln("f");
}

proc main() {
  var x = 1;
  var y = 2;
  var z = 3;
  f(x,y,z);        // should print "f"
  f(x,y, named=z); // should print "f with named"
}
