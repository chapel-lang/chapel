proc notParam(i: int) {
  return i;
}

class Foo {
  param field = notParam(1);
}

var f = new unmanaged Foo();
delete f;
