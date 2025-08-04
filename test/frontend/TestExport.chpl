
use Print;

require "TestExport.h";

extern proc helper(): int;

proc main() {
  println(helper());
}

export proc chpl_helper(): int {
  return 42;
}
