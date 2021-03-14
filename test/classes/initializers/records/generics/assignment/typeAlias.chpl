record Generic {
  type eltType;
  var value: eltType;

  proc init(type eltType, value: eltType) where !isSubtype(value.type, Generic) {
    this.eltType = eltType;
    this.value   = value;
  }
}

type Concrete = Generic(string);

var foo = new Concrete("hello");
writeln(foo);
