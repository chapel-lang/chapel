class MyClass { }

writeln(isSubtype(MyClass, MyClass));
writeln(isSubtype(borrowed MyClass, borrowed MyClass));

record R { type t; }

writeln(isSubtype(R, R));
writeln(isSubtype(R(int), R));
