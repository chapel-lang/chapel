require "foo.h", "bar.h", "bar.c", "baz.h", "baz.o";

extern proc foo(x: c_int);
extern proc bar(x: c_int);
extern proc baz(x: c_int);

//
// These cases check that the 'use' stuff doesn't stomp on
// command-line options
//
extern proc foo_cl(x: c_int);
extern proc bar_cl(x: c_int);

foo(42);
bar(53);
baz(64);
foo_cl(75);
bar_cl(86);
