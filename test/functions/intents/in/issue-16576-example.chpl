record R {
  var x: int;
  proc init=(rhs: int) {
    this.x = rhs;
  }
}
operator =(ref lhs:R, const ref rhs:int) {
  lhs.x = rhs;
}

proc f(in arg: R) { }
{
  f(1); // expecting failure to resolve
        // unless/until an implicit conversion is opted into separately
}
