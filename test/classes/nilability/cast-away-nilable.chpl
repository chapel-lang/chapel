class MyClass { var x:int; }

var nilable:MyClass?;
nilable = (new owned MyClass(2)).borrow();
writeln(nilable);

var notNilable = nilable:MyClass;
writeln(notNilable);

nilable = nil;
notNilable = nilable:MyClass;
// Expecting an error thrown
