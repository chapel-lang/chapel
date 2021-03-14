record Foo {
  var x : int;
  var y : int;
}

export proc dothis(r : Foo) : Foo {
  var ret:Foo;
  ret.x = r.x + 1;
  ret.y = r.y + 2;
  return ret;
}
