class MyClass { var x:int; }

assert(nil == nil);
assert(!(nil != nil));

var nilable:MyClass?;
writeln(nilable);

nilable = nil;
writeln(nilable);

assert(nilable == nil);
assert(nil == nilable);

nilable = (new owned MyClass(2)).borrow();
writeln(nilable);

assert(nilable != nil);
assert(nil != nilable);
