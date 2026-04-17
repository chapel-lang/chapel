record R {
  type T;
  const x: int;

  proc init(type T, x: int) {
    this.T = T;
    this.x = x;
    init this;
  }
  proc init(type T) {
    this.init(T, 7);
  }
}

// make sure there is no infinite loop in the compiler when
// resolving R's init function, which can call itself. array types seem to
// trigger this issue
var r1 = new R(int);
var r2: [0..4] R(int);
writeln(r1, "\n", r2);
