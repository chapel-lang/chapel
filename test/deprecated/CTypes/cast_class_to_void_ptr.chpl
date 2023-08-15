use CTypes;

class Foo {
  var x: int;
}

var myowned : owned Foo = new owned Foo();
var mynilableowned : owned Foo? = nil;
var myunmanaged : unmanaged Foo = new unmanaged Foo();
var mynilableunmanaged : unmanaged Foo? = new unmanaged Foo();

var ptr = myowned:c_ptr(void);
ptr = mynilableowned:c_ptr(void);
ptr = myunmanaged:c_ptr(void);
ptr = mynilableunmanaged:c_ptr(void);

delete myunmanaged;
delete mynilableunmanaged;
