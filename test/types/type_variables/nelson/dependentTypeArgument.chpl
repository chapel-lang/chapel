class A {
  proc hello() { writeln("hello from A!"); }
}

proc bar(type T) {
  var t = new T();
  t.hello();
}


proc main() {
  bar(A);
}
