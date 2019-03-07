extern {
 #include "externRecord.h"
}

proc main() {
  var myR: R;
  myR.x = 42;

  extern proc foo(x: R);
  extern proc bar(ref x: R);

  on Locales[numLocales-1] {
    baz(myR);
  }
}

proc baz(myRArg) {
  foo(myRArg);
  //  bar(myRArg);
}