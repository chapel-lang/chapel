require "foo.c", "foo.h";

extern proc bar(): c_int;

proc callBar(): c_int {
  return bar();
}
