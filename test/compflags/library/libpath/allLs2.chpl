require "foo.h";
require "bar.h", "-lbar";;
require "baz.h";

extern proc foo();
extern proc bar();
extern proc baz();

writeln("In main");
foo();
bar();
baz();
