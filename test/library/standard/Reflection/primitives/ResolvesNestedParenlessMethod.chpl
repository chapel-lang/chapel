

record inner {
  proc doSomeWork() { return; }
}

record outer {
  var _field = new inner();
  inline proc parenlessMethod { return _field; }
}

proc test() {
  var obj = new outer();
  param yes = __primitive("resolves", obj.parenlessMethod.doSomeWork());
  assert(yes);
}
test();

