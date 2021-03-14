class MyClass { var x:int; }

assert(nil == nil);
assert(!(nil != nil));

var nilable:borrowed MyClass?;
writeln(nilable.type:string);
writeln(nilable);

nilable = nil;
writeln(nilable);

assert(nilable == nil);
assert(nil == nilable);

var ownedTmp = new owned MyClass(2);
nilable = ownedTmp.borrow();
writeln(nilable);

assert(nilable != nil);
assert(nil != nilable);

var nilableu:unmanaged MyClass?;
writeln(nilableu.type:string);
writeln(nilableu);

nilableu = nil;
writeln(nilableu);

assert(nilableu == nil);
assert(nil == nilableu);

nilableu = new unmanaged MyClass(2);
writeln(nilableu);

assert(nilableu != nil);
assert(nil != nilableu);

delete nilableu;
