record Foo {
  var x : int;
  var y : int;
}

pragma "export" proc dothis(r : Foo) : Foo {
  r.x = r.x + 1;
  r.y = r.y + 2;
  return r;
}
