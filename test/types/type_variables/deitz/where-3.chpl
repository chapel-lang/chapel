fun foo(s) where s:seq { writeln("is a sequence"); }

fun foo(s) where !s:seq { writeln("is not a sequence"); }

foo(1);
foo(2.0);
foo((/1, 2/));
foo((1, 2));
