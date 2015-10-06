record Foo {
  var x : int;
  var y : int;
}

export proc dothis(r : Foo) : Foo {
  r.x = r.x + 1;
  r.y = r.y + 2;
  return r;
}
