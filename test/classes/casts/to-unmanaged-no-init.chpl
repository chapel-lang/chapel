
class A {}

var myOwned = new A?();

// this should be an error, `myOwned` is owned
{ var myUnmanaged: unmanaged A? = myOwned; }
// this should be an error, `myOwned!` is borrowed
{ var myUnmanaged: unmanaged A? = myOwned!; }
// this is allowed
{ var myUnmanaged: unmanaged A? = myOwned!:unmanaged; }


var myShared = new shared A?();

// this should be an error, `myShared` is shared
{ var myUnmanaged: unmanaged A? = myShared; }
// this should be an error, `myShared!` is borrowed
{ var myUnmanaged: unmanaged A? = myShared!; }
// this is allowed
{ var myUnmanaged: unmanaged A? = myShared!:unmanaged; }
