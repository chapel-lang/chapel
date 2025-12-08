use CTypes;

require "invalid-types.h";
require "invalid-types.c";
extern proc myFunc(ref arr: real);

proc main() {
  var r = 1.0;
  myFunc(r);
}
