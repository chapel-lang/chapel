require "foo.h", "-lfoo";

extern proc foo();

export proc callsFoo() {
  foo();
}
