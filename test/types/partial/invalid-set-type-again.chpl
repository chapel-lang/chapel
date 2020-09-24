record R {
  type fixed; //never allowed to change
  type p;
  var x: int;
}

proc R.init=(rhs: R) {
  this.fixed = rhs.fixed; // what happens if this.fixed is already false and rhs.fixed is true?

  // this one is set to the rhs if not provided already in the lhs
  this.p = if this.type.p != ? then this.type.p else rhs.p;

  this.x = rhs.x;
}

proc =(ref lhs:R, const ref rhs:R) {
  lhs.x = rhs.x;
}

proc main() {
  var start = new R(int, int, 1);
  writeln(start.type:string, " ", start);

  var b: R(real, real) = start; // init= -- expecting an error
  writeln(b.type:string, " ", b);
}
