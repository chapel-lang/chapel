type t = nothing;

proc foo(a: t = none) { return a; }  // fails with compiler bug

foo();

proc bar(a: nothing = none) { return a; }  // works
bar();
