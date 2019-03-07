record bar {
  type t;
  var aField: t;

  proc init(type t) {
    this.t = t;
    if t == int(8) {
      this.aField = -1;
    }
  }
}

var foo: bar(int(32));
var baz: bar(int(8));
writeln(foo);
writeln(baz);
