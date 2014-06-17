proc foo() return (1, 2.0);

proc bar(x = foo()(2)) return x;

writeln(bar());
