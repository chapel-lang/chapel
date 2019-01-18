class foo {
  var a: int;
}

proc bar(type t) {
  var M = new unmanaged t();
  return M;
}

var x = bar(foo);

delete x;

