
require 'extern-record-init.h';

extern record R {
  var b : int;
  var c : int;
}

extern proc mutateR(ref r : R) : void;
extern proc printR(const ref r : R) : void;

proc main() {
  var r = new R(4, 10);
  mutateR(r);
  var x = r;
  printR(r);
  printR(x);
}
