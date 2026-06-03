use CTypes;
require "invalid-num-args.h";
require "invalid-num-args.c";
extern proc myFunc();

proc main() {
  myFunc();
}
