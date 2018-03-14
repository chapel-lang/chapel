class Generic {
  type eltType;
  var value: eltType;

  proc init(value: ?eltType) {
    this.eltType = eltType;
    this.value   = value;
  }
}

type Concrete = Generic(string);

var foo = new Concrete("hello");
writeln(foo);
delete foo;
