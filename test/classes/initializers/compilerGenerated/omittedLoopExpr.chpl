class Foo {
  var D: domain(1);
  var A: [D] real = [i in D] i;
}

var foo = new Foo({1..10});
writeln(foo);
delete foo;
