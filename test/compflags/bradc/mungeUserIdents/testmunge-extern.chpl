extern proc foo();

extern record bar {
  var foo: real;
}

extern var baz: bar;
var b: bar;

foo();
writeln(baz);
writeln(baz.foo);
b.foo = 1.2;
writeln(b);

