record R {
  proc init() { writeln("no-arg R initializer"); }
}
var r: R;

record Q {
  proc init(i:int=0) { writeln("Q(i) initializer"); }
}
var q: Q;
