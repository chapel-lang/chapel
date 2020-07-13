record R {
  param fixed; //never allowed to change
  param p;
  var x: int;
}

proc R.init=(rhs: R) {
  // this one is not allowed to vary between lhs/rhs
  if this.type.fixed != ? {
    if this.type.fixed != rhs.fixed {
      compilerError("cannot change fixed field in initialization");
    }
  }

  this.fixed = rhs.fixed;

  // this one is set to the rhs if not provided already in the lhs
  this.p = if this.type.p != ? then this.type.p else rhs.p;

  this.x = rhs.x;
}

proc =(ref lhs:R, const ref rhs:R) {
  if lhs.fixed != rhs.fixed {
    compilerError("cannot change fixed field in assignment");
  }
  lhs.x = rhs.x;
}

proc main() {
  var start = new R(true, true, 1);
  writeln(start.type:string, " ", start);

  var a: R(false, false); a; // no split-init
  a = start; // assignment across types
  writeln(a.type:string, " ", a);
}
