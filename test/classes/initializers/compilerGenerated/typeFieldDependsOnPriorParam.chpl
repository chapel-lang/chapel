class C {
  param rank : int;
  type type1;
  type type2 = rank*type1;


  proc foo() {
    var x: type1;
    var x2: type2;

    writeln("x is: ", x);
    writeln("x2 is: ", x2);
  }
}

var c = new unmanaged C(2,int);
c.foo();

var c2 = new unmanaged C(3,real);
c2.foo();

delete c;
delete c2;
