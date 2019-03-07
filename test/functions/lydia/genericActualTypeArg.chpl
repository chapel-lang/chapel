proc bar(type t) {
  writeln(t: string);
}

bar(int);
bar(Foo);


class Foo {
  var x;
}
