var b: _bytes = " base bytes ";

printValAndType(b+3:_bytes);
printValAndType(3:_bytes+b);

enum Test {Foo, Bar};

printValAndType(b+Test.Foo:_bytes);
printValAndType(Test.Bar:_bytes+b);

printValAndType(b+true:_bytes);
printValAndType(false:_bytes+b);

printValAndType(b*4);

proc printValAndType(v: ?t) {
  writeln("Value = ", v, " Type = ", t:string);
}
