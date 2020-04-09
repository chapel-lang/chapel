use SysCTypes;

config param baz_h = "baz.h",
             baz_c = "baz.c";

require "foo.c", "foo.h", baz_h, baz_c;

extern proc bar(): c_int;
extern proc baz();

proc callBar(): c_int {
  baz();
  return bar();
}
