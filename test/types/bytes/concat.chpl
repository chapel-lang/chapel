var b: _bytes = " base bytes ";

printValAndType(b+3);
printValAndType(3+b);

enum Test {Foo, Bar};

printValAndType(b+Test.Foo);
printValAndType(Test.Bar+b);

printValAndType(b+true);
printValAndType(false+b);

printValAndType(b*4);

proc printValAndType(v: ?t) {
  writeln("Value = ", v, " Type = ", t:string);
}
