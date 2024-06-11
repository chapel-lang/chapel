use Types;

record R {
  type T;
  var x: T;
  proc init(type T, arg: T) {
    this.T = T;
    this.x = arg;
  }
  proc init(arg: ?t) {
    this.T = t;
    this.x = arg;
  }
}

type D = [0..#2] R(real);

proc main() {
  var x: D = [new R(1.0), new R(2.0)];
  writeln(x);
  assert(!isDefaultInitializable(R(real)));
}
