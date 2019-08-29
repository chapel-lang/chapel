var b = b" base bytes ";

printValAndType(b+3:bytes);
printValAndType(3:bytes+b);

enum Test {Foo, Bar};

printValAndType(b+Test.Foo:bytes);
printValAndType(Test.Bar:bytes+b);

printValAndType(b+true:bytes);
printValAndType(false:bytes+b);

printValAndType(b*4);

proc printValAndType(v: ?t) {
  writeln("Value = ", v, " Type = ", t:string);
}
