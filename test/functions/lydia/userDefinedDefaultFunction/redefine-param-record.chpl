record bar {
  param size: int;
  var whatev = 5;

  proc init(param size : int) {
    this.size = size;
    if size == 3 {
      this.whatev = 7;
    }
  }
}

var foo: bar(3);
var baz: bar(2);
writeln("(size = ", foo.size, ", whatev = ", foo.whatev, ")");
writeln("(size = ", baz.size, ", whatev = ", baz.whatev, ")");
