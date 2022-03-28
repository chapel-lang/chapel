require "header.h";
use CTypes;

extern proc returnVoidFromVoidArg(arg:c_int);

proc main() {
  returnVoidFromVoidArg(1);
}
