
union U {
  var x: string;
  var y: real;
}
var u: U;
u.x = "";

record functor {
  proc this(x: real) {
  }
}

u.visit(new functor(), new functor());
