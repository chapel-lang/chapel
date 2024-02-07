
class A {}

proc foo(a: unmanaged A?) {
  writeln(a);
}

var myOwned = new A?();

// this should be an error, myOwned is owned
foo(myOwned);
// this should be an error, myOwned is borrowed
foo(myOwned!);
// this is allowed
foo(myOwned!:unmanaged);

var myShared = new shared A?();

// this should be an error, myShared is shared
foo(myShared);
// this should be an error, myShared is borrowed
foo(myShared!);
// this is allowed
foo(myShared!:unmanaged);
