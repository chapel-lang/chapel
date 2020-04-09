record Generic {
  type eltType;
  var value: eltType;

  proc init(type eltType, value: eltType) {
    this.eltType = eltType;
    this.value   = value;
  }
}

type Concrete = Generic(int);

var foo = new Concrete("hello");
writeln(foo);
