module IntUIntOpsFirst {
  config param safeCast = true;

  proc +(x: int, y: uint) {
    if safeCast then
      return x + y.safeCast(int);
    else
      return x + y:int;
  }

  proc +(x: uint, y: int) {
    if safeCast then
      return x + y.safeCast(uint);
    else
      return x + y:uint;
  }
}
