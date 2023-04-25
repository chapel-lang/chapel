proc foo() do return 42;
proc bar() do writeln("42");
proc baz() do [i in 1..1] writeln("42");
proc boo() do forall i in 1..1 do writeln(42);
proc goo() do begin writeln(42);
proc hoo() do cobegin { writeln(42); writeln(42); }

writeln(foo());
bar();
baz();
boo();
goo();
hoo();
