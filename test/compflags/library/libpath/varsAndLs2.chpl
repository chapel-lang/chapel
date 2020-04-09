require "foo.h", "-lfoo";
require "bar.h", "-lbar";
require "baz.h", "-lbaz";

extern proc foo();
extern proc bar();
extern proc baz();

writeln("In main");
foo();
bar();
baz();
