
var resource = 0;

record R {
  var x : int;

  proc init() {
    resource += 1;
    x = 0;
    writeln("R.init()");
  }

  proc init(other:R) {
    resource += 1;
    this.x = 1;
    writeln("R.init(R): ", x);
  }

  proc deinit() {
    resource -= 1;
    writeln("R.deinit(): ", x);
  }
}

pragma "use default init"
class CC {
  var r : R;
}

{
  writeln("----- Concrete Class -----");
  var cc = new CC();
  delete cc;
}
writeln("resource = ", resource);

pragma "use default init"
class GC {
  type t;
  var r : R;
}

{
  writeln("----- Generic Class -----");
  var gc = new GC(int);
  delete gc;
}
writeln("resource = ", resource);

pragma "use default init"
record CR {
  var r : R;
}

{
  writeln("----- Concrete Record -----");
  var cr = new CR();
}
writeln("resource = ", resource);

pragma "use default init"
record GR {
  type t;
  var r : R;
}

{
  writeln("----- Generic Record -----");
  var gr = new GR(int);
}
writeln("resource = ", resource);
