use GlobalAtomicObject;

class C { var c : int; }
var a = new C(1);
var b = new C(2);
var x = new GlobalAtomicObject(C); // atomic C
var result : bool;

x.write(a);
result = x.compareExchange(a, b);
assert(result);
assert(x.read() == b);

// Note that you can call 'delete' on the object while having it still be present
// in the descriptor table. This may be okay for most cases where memory is not
// an issue since reused addresses will hash to the same node and that node will
// already point to the valid object the user is attempting to insert. However if
// memory is an issue, one can call '_delete' to ensure that the node itself is removed
// and can be recycled for later use.
delete a;
delete b;

// Is Safe because we only use the pointer itself. However, in cases where 'a' and 'b'
// can be reused by the same GlobalAtomicObject, then this could cause undefined behavior
// where another concurrent task adds the same pointer. In those cases, you should call
// '_delete' before 'delete'.
x._delete(a);
x._delete(b);

// As well, when GlobalAtomicObject goes out of scope, all nodes it had in use also
// get deleted...
