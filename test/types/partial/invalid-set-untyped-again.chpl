record R {
  var fixed; //never allowed to change
  var x: int;
}

proc R.init=(rhs: R) {
  this.fixed = rhs.fixed; // what happens if this.fixed is already false and rhs.fixed is true?

  this.x = rhs.x;
}

proc =(ref lhs:R, const ref rhs:R) {
  lhs.x = rhs.x;
}

proc main() {
  var start = new R(1, 1);
  writeln(start.type:string, " ", start);

  var b: R(real) = start; // init= -- expecting an error
  writeln(b.type:string, " ", b);
}
