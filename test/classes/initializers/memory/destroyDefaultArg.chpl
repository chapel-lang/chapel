
var resource = 0;

record R {
  var x : int;

  proc init() {
    resource += 1;
    x = 0;
    writeln("R.init()");
  }

  proc init=(other:R) {
    resource += 1;
    this.x = 1;
    writeln("R.init=(R): ", x);
  }

  proc deinit() {
    resource -= 1;
    writeln("R.deinit(): ", x);
  }
}

class CC {
  var r : R;
}

{
  writeln("----- Concrete Class -----");
  var cc = new unmanaged CC();
  delete cc;
}
writeln("resource = ", resource);

class GC {
  type t;
  var r : R;
}

{
  writeln("----- Generic Class -----");
  var gc = new unmanaged GC(int);
  delete gc;
}
writeln("resource = ", resource);

record CR {
  var r : R;
}

{
  writeln("----- Concrete Record -----");
  var cr = new CR();
}
writeln("resource = ", resource);

record GR {
  type t;
  var r : R;
}

{
  writeln("----- Generic Record -----");
  var gr = new GR(int);
}
writeln("resource = ", resource);
