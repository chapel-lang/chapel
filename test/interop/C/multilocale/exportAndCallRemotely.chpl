require "callbackfromC.h";

export proc foo(ref x: int) {
  writeln("foo() got: ", x);
}

proc main() {
  var x = 42: int;

  extern proc bar(ref x: int);

  foo(x);
  x += 1;
  bar(x);
  on Locales[numLocales-1] {
    x += 1;
    foo(x);
    x += 1;
    bar(x);
    x += 1;
  }
  foo(x);
  x += 1;
  bar(x);
}
