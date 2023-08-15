use CTypes;

class Foo {
  var x: int;
}

var myowned : owned Foo = new owned Foo();
var mynilableowned : owned Foo? = nil;
var myunmanaged : unmanaged Foo = new unmanaged Foo();
var mynilableunmanaged : unmanaged Foo? = new unmanaged Foo();

// Trigger deprecation warnings for casts to c_ptr(void), but also ensure the
// new proper route of c_ptrTo has the same behavior as the old cast way.
writeln(c_ptrTo(myowned):string == myowned:c_ptr(void):string);
writeln(c_ptrTo(mynilableowned):string == mynilableowned:c_ptr(void):string);
writeln(c_ptrTo(myunmanaged):string == myunmanaged:c_ptr(void):string);
writeln(c_ptrTo(mynilableunmanaged):string == mynilableunmanaged:c_ptr(void):string);

delete myunmanaged;
delete mynilableunmanaged;
