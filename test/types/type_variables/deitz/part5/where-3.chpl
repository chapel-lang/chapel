def foo(s) where s:seq { writeln("is a sequence"); }

def foo(s) where !s:seq { writeln("is not a sequence"); }

foo(1);
foo(2.0);
foo(_seq(1, 2));
foo((1, 2));
