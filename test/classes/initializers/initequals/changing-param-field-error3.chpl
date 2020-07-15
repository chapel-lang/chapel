record R {
  param fixed; //never allowed to change
  param p;
  var x: int;
}

proc R.init=(rhs: R) {
  this.fixed = rhs.fixed;

  // this one is set to the rhs if not provided already in the lhs
  this.p = if this.type.p != ? then this.type.p else rhs.p;

  this.x = rhs.x;
}

proc =(ref lhs:R, const ref rhs:R) {
  lhs.x = rhs.x;
}

proc main() {
  var start = new R(true, true, 1);
  writeln(start.type:string, " ", start);

  var b: R(false, false) = start; // init= -- expecting an error
  writeln(b.type:string, " ", b);
}
