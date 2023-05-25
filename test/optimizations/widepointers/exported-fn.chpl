// Even though foo() is not invoked directly, it can be invoked
// on a non-0 locale  from bar(). Ensure that 'globalArr' is widened.

extern proc bar();

var globalArr : [1..1] real;

export proc foo() {
  writeln(globalArr.size);
}

proc main() {
  on Locales[1] do {
    bar();
  }
}
