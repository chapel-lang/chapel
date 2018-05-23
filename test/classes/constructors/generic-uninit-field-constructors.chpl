/////////////////////////////////
// user-defined constructors,
// the compiler forces them to have formals corresponding
// to the generic fields (see sample compiler error messages in comments)
// (and callers to pass values to those formals,
//  via the resolution mechanism - because they do not have defaults?)
// the actuals DO initialize the like-formal-named fields
/////////////////////////////////

class A {
  type t;
  var x:t;
  proc A(type t) { }
  // constructor for class 'A' requires a generic argument called 't'
}

var a = new unmanaged A(real);

writeln(a);

delete a;






class B {
  param p;
  proc B(param p) { }
  // constructor for class 'B' requires a generic argument called 'p'
}

var b = new unmanaged B("hello-user");

writeln("{p = ", b.p, "}");

delete b;





class C {
  const cst;
  proc C(cst) { }
  // constructor for class 'C' requires a generic argument called 'cst'
}

var c = new unmanaged C(1.2);

writeln(c);

delete c;







class D {
  var vbl;
  proc D(vbl) { }
  // constructor for class 'D' requires a generic argument called 'vbl'
}

var d = new unmanaged D(7);

writeln(d);

delete d;





/////////////////////////////////
// default constructors,
// the compiler forces callers to pass values to the formals
// corresponding to the generic fields because they do not have defaults
/////////////////////////////////

class Ad {
  type t;
  var x:t;
}

var ad = new unmanaged Ad(real);

writeln(ad);

delete ad;






class Bd {
  param p;
}

var bd = new unmanaged Bd("hello-default");

writeln("{p = ", bd.p, "}");

delete bd;







class Cd {
  const cst;
}

var cd = new unmanaged Cd(1.33);

writeln(cd);

delete cd;









class Dd {
  var vbl;
}

var dd = new unmanaged Dd(55);

writeln(dd);

delete dd;
