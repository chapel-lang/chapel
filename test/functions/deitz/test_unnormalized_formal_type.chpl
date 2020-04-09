proc foo() return (1, 2.0);

proc bar(x = foo()(1)) return x;

writeln(bar());
