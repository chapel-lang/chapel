
config const cond : bool = true;

class C { }

record R {
  var x : int;
  var potentialAlias: unmanaged C?;
  proc init() {
    this.x = 0;
    writeln("R.init: ", x);
  }
  proc init(x:int) {
    this.x = x;
    writeln("R.init(int): ", x);
  }
  proc init=(other:R) {
    this.x = other.x + 1;
    writeln("R.init=(R): ", x);
  }
  proc deinit() {
    writeln("R.deinit: ", x);
  }
}

proc main() {
  // This test exists to ensure that we do not free the result of the nested
  // if-expr before the outer one 'returns'.
  //
  // BHARSH INIT TODO: Looks like there is an unnecessary copy on the inner
  // result.
  var x = if cond then
            if cond then new R(100) else new R(200)
          else
            new R(300);
}
