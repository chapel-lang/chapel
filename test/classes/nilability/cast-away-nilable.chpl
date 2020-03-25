class MyClass { var x:int; }

var nilable:borrowed MyClass? = (new owned MyClass(2)).borrow();
writeln(nilable);

var notNilable = nilable:borrowed MyClass;
writeln(notNilable);


nilable = nil;
notNilable = nilable:borrowed MyClass;
// Expecting an error thrown
