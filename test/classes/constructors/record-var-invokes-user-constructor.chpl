record R {
  proc R() { writeln("no-arg R constructor"); }
}
var r: R;

record Q {
  proc Q(i:int=0) { writeln("Q(i) constructor"); }
}
var q: Q;
