proc foo() {
  writeln("Hello!");
}

proc bar(f) {
  f();
}

bar(foo);

//foo();