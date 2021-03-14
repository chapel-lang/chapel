
config param case : int = 0;

record D {
  type T;
  param b : bool;
  const x;
  param p : if b then T else x.type;
}

class Parent {
  type T;
}

class Child : Parent {
  param p : T;
}

proc main() {
  if case == 0 {
    type X = D(int, p=1024);
    writeln(X:string);
  } else if case == 1 {
    type X = Child(p=1024);
    writeln(X:string);
  }
}
