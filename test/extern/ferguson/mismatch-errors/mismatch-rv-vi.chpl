require "header.h";
use SysCTypes;

extern proc returnVoidFromVoidArg(arg:c_int);

proc main() {
  returnVoidFromVoidArg(1);
}
