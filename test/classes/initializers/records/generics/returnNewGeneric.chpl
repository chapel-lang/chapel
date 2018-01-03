record Option {
  type eltType;
  var state: bool;
  var value: eltType;

  proc init(value: ?eltType) where !value: Option {
    this.eltType = eltType;
    this.state   = true;
    this.value   = value;
    super.init();
  }
}

proc Some(value: ?eltType) {
  return new Option(value);
}

assert(Some(42) == Some(42));
