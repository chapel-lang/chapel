record Option {
  type eltType;
  var state: bool;
  var value: eltType;

  proc init(value: ?eltType) where !isSubtype(value.type, Option) {
    this.eltType = eltType;
    this.state   = true;
    this.value   = value;
  }
}

proc Some(value: ?eltType) {
  return new Option(value);
}

assert(Some(42) == Some(42));
