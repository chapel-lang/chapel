proc foo() do return (1, 2.0);

proc bar(x = foo()(1)) do return x;

writeln(bar());
