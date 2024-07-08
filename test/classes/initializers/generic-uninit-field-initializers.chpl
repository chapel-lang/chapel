
/////////////////////////////////
// user-defined initializers.
// Though constructors required formals corresponding to generic fields
// which were then used to initialize the like-formal-named fields,
// initializers do not require this.  But they are a good example of generics
// working.
/////////////////////////////////

class A {
  type t;
  var x:t;
  proc init(type t) {
    this.t = t;
  }
  // initializer for class 'A' requires a generic argument called 't'
}
var ownA = new owned A(real);
var a = ownA.borrow();

writeln(a);








class B {
  param p;
  proc init(param p) {
    this.p = p;
  }
  // initializer for class 'B' requires a generic argument called 'p'
}
var ownB = new owned B("hello-user");
var b = ownB.borrow();

writeln("{p = ", b.p, "}");







class C {
  const cst;
  proc init(cst) {
    this.cst = cst;
  }
  // initializer for class 'C' requires a generic argument called 'cst'
}
var ownC = new owned C(1.2);
var c = ownC.borrow();

writeln(c);









class D {
  var vbl;
  proc init(vbl) {
    this.vbl = vbl;
  }
  // initializer for class 'D' requires a generic argument called 'vbl'
}
var ownD = new owned D(7);
var d = ownD.borrow();

writeln(d);







/////////////////////////////////
// default initializers,
// the compiler forces callers to pass values to the formals
// corresponding to the generic fields because they do not have defaults
/////////////////////////////////

class Ad {
  type t;
  var x:t;
}
var ownAd = new owned Ad(real);
var ad = ownAd.borrow();

writeln(ad);








class Bd {
  param p;
}
var ownBd = new owned Bd("hello-default");
var bd = ownBd.borrow();

writeln("{p = ", bd.p, "}");









class Cd {
  const cst;
}
var ownCd = new owned Cd(1.33);
var cd = ownCd.borrow();

writeln(cd);











class Dd {
  var vbl;
}
var ownDd = new owned Dd(55);
var dd = ownDd.borrow();

writeln(dd);
