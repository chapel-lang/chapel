// Modified from
// test/classes/constructors/generic-uninit-field-constructors.chpl

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
    super.init();
  }
  // initializer for class 'A' requires a generic argument called 't'
}

var a = new A(real);

writeln(a);

delete a;






class B {
  param p;
  proc init(param p) {
    this.p = p;
    super.init();
  }
  // initializer for class 'B' requires a generic argument called 'p'
}

var b = new B("hello-user");

writeln("{p = ", b.p, "}");

delete b;





class C {
  const cst;
  proc init(cst) {
    this.cst = cst;
    super.init();
  }
  // initializer for class 'C' requires a generic argument called 'cst'
}

var c = new C(1.2);

writeln(c);

delete c;







class D {
  var vbl;
  proc init(vbl) {
    this.vbl = vbl;
    super.init();
  }
  // initializer for class 'D' requires a generic argument called 'vbl'
}

var d = new D(7);

writeln(d);

delete d;





/////////////////////////////////
// default initializers,
// the compiler forces callers to pass values to the formals
// corresponding to the generic fields because they do not have defaults
/////////////////////////////////

class Ad {
  type t;
  var x:t;
}

var ad = new Ad(real);

writeln(ad);

delete ad;






class Bd {
  param p;
}

var bd = new Bd("hello-default");

writeln("{p = ", bd.p, "}");

delete bd;







class Cd {
  const cst;
}

var cd = new Cd(1.33);

writeln(cd);

delete cd;









class Dd {
  var vbl;
}

var dd = new Dd(55);

writeln(dd);

delete dd;
