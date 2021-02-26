proc g( x:int ...2, named:int) {
  writeln("g with named");
}
proc g( x:int ...3) {
  writeln("g");
}

proc main() {
  var x = 1;
  var y = 2;
  var z = 3;
  g(x,y,z);        // ambiguity error
  g(x,y, named=z);
}
