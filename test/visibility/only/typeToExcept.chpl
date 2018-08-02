class foo {
  var fieldA = 13;

  proc someMethod(x: int) {
    return fieldA * x;
  }
}

proc bar() {
  return new unmanaged foo(2);
}
