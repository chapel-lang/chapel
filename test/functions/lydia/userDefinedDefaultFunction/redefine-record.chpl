record bar {
  var aField: int;

  proc init() {
    this.aField = 4;
  }
}

var foo: bar;
writeln(foo);
