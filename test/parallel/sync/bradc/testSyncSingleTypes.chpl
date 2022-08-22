config type t = complex(64);
config param testSingle = false;

proc main() {
  var x: if testSingle then single t else sync t;
  // writeln(x.readXX());
}

record R {
  var x: int;
  var y: int;
}
