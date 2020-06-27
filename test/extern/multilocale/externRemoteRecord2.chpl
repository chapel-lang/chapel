extern {
 #include "externRecord.h"
}

proc main() {
  var myR: R;
  myR.x = 42;

  extern proc foo(in x: R);
  extern proc bar(ref x: R);

  on Locales[numLocales-1] {
    foo(myR);
    bar(myR);
  }
}
