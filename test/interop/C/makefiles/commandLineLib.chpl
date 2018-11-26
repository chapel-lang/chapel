require "foo.h";

extern proc foo();

export proc callsFoo() {
  foo();
}
