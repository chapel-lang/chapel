// test from issue #18672

use SysCTypes;

extern "strlen"        proc f1(c_string): size_t;
extern "strlen" inline proc f2(c_string): size_t;

writeln(f1("what's the length of this?"));
writeln(f2("and of this?"));
