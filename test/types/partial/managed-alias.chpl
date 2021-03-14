
class MyClass {
  type T;
  param n : int;
}

proc main() {
  {
    type A = MyClass;
    type B = unmanaged A;
    writeln(B:string);
    type C = B(int);
    writeln(C:string);
    type D = C(5);
    writeln(D:string);

    type X = B(n=5);
    writeln(X:string);
    type Y = X(int);
    writeln(Y:string);
  }
  writeln();
  {
    type A = MyClass;
    type B = borrowed A;
    writeln(B:string);
    type C = B(int);
    writeln(C:string);
    type D = C(5);
    writeln(D:string);

    type X = B(n=5);
    writeln(X:string);
    type Y = X(int);
    writeln(Y:string);
  }
  writeln();
  {
    type A = MyClass;
    type B = owned A;
    writeln(B:string);
    type C = B(int);
    writeln(C:string);
    type D = C(5);
    writeln(D:string);

    type X = B(n=5);
    writeln(X:string);
    type Y = X(int);
    writeln(Y:string);
  }
  writeln();
  {
    type A = MyClass;
    type B = shared A;
    writeln(B:string);
    type C = B(int);
    writeln(C:string);
    type D = C(5);
    writeln(D:string);

    type X = B(n=5);
    writeln(X:string);
    type Y = X(int);
    writeln(Y:string);
  }
}
