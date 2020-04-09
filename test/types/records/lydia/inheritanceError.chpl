record Foo {
  var x = 3;
}

record Bar : Foo {
  var y = 5;
}

var bar = new Bar();
writeln(bar);
