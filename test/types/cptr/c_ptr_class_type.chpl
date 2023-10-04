use CTypes;

class Foo {
  var x: int;
  proc getX() : int {
    return x;
  }
}

var myowned : owned Foo = new owned Foo();
var mynilableowned : owned Foo? = nil;

var myshared : shared Foo = new shared Foo();
var mysharedsecondref : shared Foo = myshared;

var myborrowed : borrowed Foo = myowned.borrow();

var myunmanaged : unmanaged Foo = new unmanaged Foo(5);
var mynilableunmanaged : unmanaged Foo? = new unmanaged Foo();

writeln(c_ptrTo(myowned).type:string);
writeln(c_ptrToConst(myowned).type:string);
writeln();

writeln(c_ptrTo(myowned):string == c_ptrToConst(myowned):string);
writeln(c_ptrTo(mynilableowned):string == c_ptrToConst(mynilableowned):string);
writeln(c_ptrTo(myowned):string == (c_ptrTo(myowned)):string);
writeln(c_ptrTo(mynilableowned):string == (c_ptrTo(mynilableowned)):string);
writeln();

writeln(c_ptrTo(myshared):string == c_ptrToConst(myshared):string);
writeln(c_ptrTo(myshared):string == (c_ptrTo(myshared)):string);
writeln(c_ptrTo(myshared):string == (c_ptrTo(mysharedsecondref)):string);
writeln(c_ptrTo(mysharedsecondref):string == (c_ptrTo(mysharedsecondref)):string);
writeln();

writeln(c_ptrTo(myborrowed):string == c_ptrToConst(myborrowed):string);
writeln(c_ptrTo(myborrowed):string == (c_ptrTo(myborrowed)):string);
writeln();

writeln(c_ptrTo(myunmanaged):string == c_ptrToConst(myunmanaged):string);
writeln(c_ptrTo(myunmanaged):string == (c_ptrTo(myunmanaged)):string);
writeln(c_ptrTo(mynilableunmanaged):string == (c_ptrTo(mynilableunmanaged)):string);
writeln();

// stack vs heap address
writeln(c_addrOf(myunmanaged):string == c_ptrTo(myunmanaged):string);
// check converting an instance to a pointer and back
writeln(myunmanaged.getX());
var heapPtrToAFoo : c_ptr(void) = c_ptrTo(myunmanaged);
writeln(heapPtrToAFoo.type:string);
var maybeReconstitutedFoo: unmanaged Foo? = heapPtrToAFoo:unmanaged Foo?;
var reconstitutedFoo:unmanaged Foo = maybeReconstitutedFoo!;
writeln(reconstitutedFoo.getX());

delete myunmanaged;
delete mynilableunmanaged;
