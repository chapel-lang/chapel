def foo() return (1, 2.0);

def bar(x = foo()(2)) return x;

writeln(bar());
