use "foo.h", "bar.h", "bar.c", "baz.h", "baz.o";

extern proc foo(x: c_int);
extern proc bar(x: c_int);
extern proc baz(x: c_int);

foo(42);
bar(53);
baz(64);
