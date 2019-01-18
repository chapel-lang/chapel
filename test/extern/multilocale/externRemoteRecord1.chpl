extern record R {
  var x: c_int;
}

require "externRecord.h";

proc main() {
  var myR: R;
  myR.x = 42;

  extern proc foo(x: R);
  extern proc bar(ref x: R);

  on Locales[numLocales-1] {
    foo(myR);
    bar(myR);
  }
}
