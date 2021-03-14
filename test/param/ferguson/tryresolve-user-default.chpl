use Reflection;

proc hi(x=0, y="a") {
  writeln(x,y);
}

var x:bool;

x = canResolve("hi");
assert(x);

x = canResolve("hi", 1);
assert(x);

x = canResolve("hi", "b");
assert(!x);  // hi("b") wouldn't resolve, need hi(y="b")

x = canResolve("hi", 1, "b");
assert(x);

proc string.mymethod(x=0, y="a") {
  writeln(x,y);
}

var obj="hi";

x = canResolveMethod(obj, "mymethod");
assert(x);

x = canResolveMethod(obj, "mymethod", 1);
assert(x);

x = canResolveMethod(obj, "mymethod", "b");
assert(!x);  // obj.mymethod("b") wouldn't resolve, need obj.mymethod(y="b")

x = canResolveMethod(obj, "mymethod", 1, "b");
assert(x);
