record Generic {
  type eltType;
  var value: eltType;

  proc init(value: ?eltType) where !value: Generic {
    this.eltType = eltType;
    this.value   = value;
  }
}

type Concrete = Generic(string);

var foo = new Concrete("hello");
writeln(foo);
