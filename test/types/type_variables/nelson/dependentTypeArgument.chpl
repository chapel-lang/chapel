class A {
  proc hello() { writeln("hello from A!"); }
}

proc bar(type T) {
  var t = new unmanaged T();

  t.hello();

  delete t;
}


proc main() {
  bar(A);
}
