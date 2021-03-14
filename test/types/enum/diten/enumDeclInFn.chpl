proc foo(i) {
  enum MyEnum {
    A = 0,
    B,
    C
  }

  writeln(MyEnum.A);
  writeln(i);
}

foo(3);
