
class Base {
  type T;
  param x : T;
}

class Child : Base {
  type U;
  param y : U;
}

class X {
  type T;
  param b : bool;
}

class Y : X {
  param p : T;
}

class Z : Y {
  type U;
  param zz : U;
}

proc main() {
  {
    type A = Child(T=int, U=real);
    writeln(A:string);
    type B = A(y=5.0);
    writeln(B:string);
    type C = B(100);
    writeln(C:string);
  }
  writeln();
  {
    type A = Z(T=int, p=100);
    writeln(A:string);
    type B = A(U=real);
    writeln(B:string);
    type C = B(false, 123.456);
    writeln(C:string);
  }
}
