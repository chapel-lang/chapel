module IntUIntOpsUInt {
  config param safeCast = true;

  proc +(x: int, y: uint) {
    if safeCast then
      return x.safeCast(uint) + y;
    else
      return x:uint + y;
  }

  proc +(x: uint, y: int) {
    if safeCast then
      return x + y.safeCast(uint);
    else
      return x + y:uint;
  }
}
