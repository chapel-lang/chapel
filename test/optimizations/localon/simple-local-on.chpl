class Foo {
  var x : int = 1;
}

proc main() {
  var f = new Foo();

  local on here do writeln(__primitive("is wide pointer", f));

  delete f;
}
