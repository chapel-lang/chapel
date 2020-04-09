enum myEnum {a=0, b, c}

record Foo {
  var arr: [1..10] myEnum;
}

proc byInIntent (in x) {
  writeln(x);
}

var foo: Foo;
byInIntent(foo);
writeln(foo);
