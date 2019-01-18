
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

var a = new borrowed A(real);

writeln(a);








class B {
  param p;
  proc init(param p) {
    this.p = p;
  }
  // initializer for class 'B' requires a generic argument called 'p'
}

var b = new borrowed B("hello-user");

writeln("{p = ", b.p, "}");







class C {
  const cst;
  proc init(cst) {
    this.cst = cst;
  }
  // initializer for class 'C' requires a generic argument called 'cst'
}

var c = new borrowed C(1.2);

writeln(c);









class D {
  var vbl;
  proc init(vbl) {
    this.vbl = vbl;
  }
  // initializer for class 'D' requires a generic argument called 'vbl'
}

var d = new borrowed D(7);

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

var ad = new borrowed Ad(real);

writeln(ad);








class Bd {
  param p;
}

var bd = new borrowed Bd("hello-default");

writeln("{p = ", bd.p, "}");









class Cd {
  const cst;
}

var cd = new borrowed Cd(1.33);

writeln(cd);











class Dd {
  var vbl;
}

var dd = new borrowed Dd(55);

writeln(dd);


