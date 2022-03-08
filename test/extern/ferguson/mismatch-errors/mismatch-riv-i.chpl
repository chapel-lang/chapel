require "header.h";
use CTypes;

extern proc returnIntFromIntArg(arg: c_int);

proc main() {
  returnIntFromIntArg(1);
}
