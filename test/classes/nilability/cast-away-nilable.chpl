class MyClass { var x:int; }

var ownNilable = new owned MyClass(2);
var nilable:borrowed MyClass? = ownNilable.borrow();
writeln(nilable);

var notNilable = nilable:borrowed MyClass;
writeln(notNilable);

nilable = nil;
notNilable = nilable:borrowed MyClass;
// Expecting an error thrown
