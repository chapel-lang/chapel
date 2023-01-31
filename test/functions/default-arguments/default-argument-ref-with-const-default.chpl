const seven = 7;
proc foo(ref x = seven) { x = 17; writeln(seven); }
foo();
writeln(seven);
